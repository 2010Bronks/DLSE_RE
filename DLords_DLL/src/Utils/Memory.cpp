#include "Memory.hpp"

#include <Windows.h>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstring>
#include <limits>
#include <vector>

#include "../HDE/public/hde32.h"

namespace U::Memory
{
	using FindMemoryCmp_t = bool(*)(const void* addr1, const void* addr2, size_t size, void* param);

	struct PatternByte
	{
		uint8_t Value = 0;
		bool Wildcard = false;
	};

	enum class RelocationKind
	{
		None,
		Call32,
		Jump32,
		Jump8,
		ConditionalJump8,
		ConditionalJump32
	};

	struct RelocationInstruction
	{
		size_t SourceOffset = 0;
		size_t SourceSize = 0;
		size_t DestinationOffset = 0;
		size_t DestinationSize = 0;
		RelocationKind Kind = RelocationKind::None;
		uint8_t Opcode = 0;
		uint8_t Opcode2 = 0;
	};

#pragma pack(push, 1)
	struct TrampolineMeta
	{
		uint32_t HookSize = 0;
		uint32_t TrampolineSize = 0;
		void* OriginalAddress = nullptr;
	};
#pragma pack(pop)

	static bool IsMemoryExecutable(const void* addr)
	{
		if (!addr)
			return false;

		MEMORY_BASIC_INFORMATION mem_info{};
		if (VirtualQuery(addr, &mem_info, sizeof(mem_info)) == 0)
			return false;

		if (mem_info.Protect == 0 || mem_info.Protect == PAGE_NOACCESS)
			return false;

		return mem_info.Protect == PAGE_EXECUTE || mem_info.Protect == PAGE_EXECUTE_READ ||
			mem_info.Protect == PAGE_EXECUTE_READWRITE || mem_info.Protect == PAGE_EXECUTE_WRITECOPY;
	}

	static bool FindMemoryCmp(const void* addr1, const void* addr2, size_t size, void*)
	{
		return std::memcmp(addr1, addr2, size) == 0;
	}

	static void* FindMemory(const void* addr_start, const void* addr_min, const void* addr_max, const void* data, size_t size,
		bool backward, ptrdiff_t offset, FindMemoryCmp_t comparator = FindMemoryCmp, void* comparator_param = nullptr)
	{
		if (!addr_start || !addr_min || !addr_max || !data || size == 0)
			return nullptr;

		auto lower = reinterpret_cast<uintptr_t>(addr_min);
		auto upper = reinterpret_cast<uintptr_t>(addr_max);
		if (lower >= upper || size > upper - lower)
			return nullptr;

		auto first = lower;
		auto last = upper - size;
		auto current = reinterpret_cast<uintptr_t>(addr_start);
		current = std::clamp(current, first, last);

		if (!comparator)
			comparator = FindMemoryCmp;

		for (;;)
		{
			auto address = reinterpret_cast<const void*>(current);
			if (comparator(address, data, size, comparator_param))
				return reinterpret_cast<void*>(current + offset);

			if (backward)
			{
				if (current == first)
					break;
				--current;
			}
			else
			{
				if (current == last)
					break;
				++current;
			}
		}

		return nullptr;
	}

	static int HexValue(char value)
	{
		if (value >= '0' && value <= '9')
			return value - '0';
		if (value >= 'a' && value <= 'f')
			return value - 'a' + 10;
		if (value >= 'A' && value <= 'F')
			return value - 'A' + 10;
		return -1;
	}

	static bool ParseSignature(std::string_view signature, std::vector<PatternByte>& pattern)
	{
		pattern.clear();
		size_t index = 0;

		while (index < signature.size())
		{
			while (index < signature.size() && std::isspace(static_cast<unsigned char>(signature[index])))
				++index;

			if (index >= signature.size())
				break;

			if (signature[index] == '?')
			{
				++index;
				if (index < signature.size() && signature[index] == '?')
					++index;
				pattern.push_back({ 0, true });
			}
			else
			{
				if (index + 1 >= signature.size())
					return false;

				int high = HexValue(signature[index]);
				int low = HexValue(signature[index + 1]);
				if (high < 0 || low < 0)
					return false;

				pattern.push_back({ static_cast<uint8_t>((high << 4) | low), false });
				index += 2;
			}

			if (index < signature.size() && !std::isspace(static_cast<unsigned char>(signature[index])))
				return false;
		}

		return !pattern.empty();
	}

	static bool MatchSignature(const uint8_t* address, const std::vector<PatternByte>& pattern)
	{
		for (size_t index = 0; index < pattern.size(); ++index)
		{
			if (!pattern[index].Wildcard && address[index] != pattern[index].Value)
				return false;
		}
		return true;
	}

	static bool IsSupportedRelative(const hde32s& instruction, RelocationKind& kind, size_t& destination_size)
	{
		kind = RelocationKind::None;
		destination_size = instruction.len;

		if ((instruction.flags & F32_RELATIVE) == 0)
			return true;

		if ((instruction.flags & F32_PREFIX_ANY) != 0)
			return false;

		if (instruction.opcode == 0xE8 && instruction.len == 5 && (instruction.flags & F32_IMM32))
		{
			kind = RelocationKind::Call32;
			destination_size = 5;
			return true;
		}

		if (instruction.opcode == 0xE9 && instruction.len == 5 && (instruction.flags & F32_IMM32))
		{
			kind = RelocationKind::Jump32;
			destination_size = 5;
			return true;
		}

		if (instruction.opcode == 0xEB && instruction.len == 2 && (instruction.flags & F32_IMM8))
		{
			kind = RelocationKind::Jump8;
			destination_size = 5;
			return true;
		}

		if (instruction.opcode >= 0x70 && instruction.opcode <= 0x7F && instruction.len == 2 && (instruction.flags & F32_IMM8))
		{
			kind = RelocationKind::ConditionalJump8;
			destination_size = 6;
			return true;
		}

		if (instruction.opcode == 0x0F && instruction.opcode2 >= 0x80 && instruction.opcode2 <= 0x8F &&
			instruction.len == 6 && (instruction.flags & F32_IMM32))
		{
			kind = RelocationKind::ConditionalJump32;
			destination_size = 6;
			return true;
		}

		return false;
	}

	static bool AnalyzeSpliceBlock(uint8_t* source, std::vector<RelocationInstruction>& instructions, size_t& patch_size,
		size_t& relocated_size)
	{
		instructions.clear();
		patch_size = 0;
		relocated_size = 0;

		while (patch_size < 5)
		{
			hde32s decoded{};
			unsigned int length = hde32_disasm(source + patch_size, &decoded);
			if (length == 0 || decoded.len == 0 || (decoded.flags & F32_ERROR) != 0)
				return false;

			RelocationKind kind{};
			size_t destination_size = 0;
			if (!IsSupportedRelative(decoded, kind, destination_size))
				return false;

			instructions.push_back({ patch_size, decoded.len, relocated_size, destination_size, kind, decoded.opcode, decoded.opcode2 });
			patch_size += decoded.len;
			relocated_size += destination_size;

			if (patch_size > 64 || instructions.size() > 32)
				return false;
		}

		return patch_size >= 5;
	}

	static bool GetRelativeTarget(const uint8_t* source, const RelocationInstruction& instruction, const uint8_t*& target)
	{
		const uint8_t* address = source + instruction.SourceOffset;
		if (instruction.Kind == RelocationKind::Jump8 || instruction.Kind == RelocationKind::ConditionalJump8)
		{
			int8_t displacement = 0;
			std::memcpy(&displacement, address + instruction.SourceSize - sizeof(displacement), sizeof(displacement));
			target = address + instruction.SourceSize + displacement;
			return true;
		}

		if (instruction.Kind == RelocationKind::Call32 || instruction.Kind == RelocationKind::Jump32 ||
			instruction.Kind == RelocationKind::ConditionalJump32)
		{
			int32_t displacement = 0;
			std::memcpy(&displacement, address + instruction.SourceSize - sizeof(displacement), sizeof(displacement));
			target = address + instruction.SourceSize + displacement;
			return true;
		}

		return false;
	}

	static bool MapRelocationTarget(const uint8_t* source, size_t patch_size, uint8_t* trampoline,
		const std::vector<RelocationInstruction>& instructions, const uint8_t* original_target, const uint8_t*& mapped_target)
	{
		auto source_address = reinterpret_cast<uintptr_t>(source);
		auto target_address = reinterpret_cast<uintptr_t>(original_target);
		if (target_address < source_address || target_address >= source_address + patch_size)
		{
			mapped_target = original_target;
			return true;
		}

		size_t target_offset = static_cast<size_t>(target_address - source_address);
		for (const auto& instruction : instructions)
		{
			if (instruction.SourceOffset == target_offset)
			{
				mapped_target = trampoline + instruction.DestinationOffset;
				return true;
			}
		}

		return false;
	}

	static void WriteRel32(uint8_t* destination, const uint8_t* target, size_t instruction_size)
	{
		intptr_t difference = reinterpret_cast<intptr_t>(target) - reinterpret_cast<intptr_t>(destination + instruction_size);
		int32_t displacement = static_cast<int32_t>(difference);
		std::memcpy(destination + instruction_size - sizeof(displacement), &displacement, sizeof(displacement));
	}

	static bool RelocateInstructions(uint8_t* source, size_t patch_size, uint8_t* trampoline,
		const std::vector<RelocationInstruction>& instructions)
	{
		for (const auto& instruction : instructions)
		{
			const uint8_t* source_instruction = source + instruction.SourceOffset;
			uint8_t* destination = trampoline + instruction.DestinationOffset;

			if (instruction.Kind == RelocationKind::None)
			{
				std::memcpy(destination, source_instruction, instruction.SourceSize);
				continue;
			}

			const uint8_t* original_target = nullptr;
			if (!GetRelativeTarget(source, instruction, original_target))
				return false;

			const uint8_t* mapped_target = nullptr;
			if (!MapRelocationTarget(source, patch_size, trampoline, instructions, original_target, mapped_target))
				return false;

			switch (instruction.Kind)
			{
			case RelocationKind::Call32:
				destination[0] = 0xE8;
				WriteRel32(destination, mapped_target, 5);
				break;
			case RelocationKind::Jump32:
			case RelocationKind::Jump8:
				destination[0] = 0xE9;
				WriteRel32(destination, mapped_target, 5);
				break;
			case RelocationKind::ConditionalJump8:
				destination[0] = 0x0F;
				destination[1] = static_cast<uint8_t>(0x80 | (instruction.Opcode & 0x0F));
				WriteRel32(destination, mapped_target, 6);
				break;
			case RelocationKind::ConditionalJump32:
				destination[0] = 0x0F;
				destination[1] = instruction.Opcode2;
				WriteRel32(destination, mapped_target, 6);
				break;
			default:
				return false;
			}
		}

		return true;
	}

	bool IsInBounds(uintptr_t addr, uintptr_t addr_lower, uintptr_t addr_upper)
	{
		return addr >= addr_lower && addr < addr_upper;
	}

	bool IsInBounds(const void* addr, const void* addr_lower, const void* addr_upper)
	{
		return IsInBounds(reinterpret_cast<uintptr_t>(addr), reinterpret_cast<uintptr_t>(addr_lower), reinterpret_cast<uintptr_t>(addr_upper));
	}

	void* PtrOffset(const void* addr, ptrdiff_t offset, bool dereference)
	{
		if (!addr)
			return nullptr;

		void* result = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(addr) + offset);
		if (dereference)
			result = *reinterpret_cast<void**>(result);
		return result;
	}

	uint8_t* FindU8(const void* addr_start, const void* addr_min, const void* addr_max, uint8_t value, bool backward, ptrdiff_t offset)
	{
		return static_cast<uint8_t*>(FindMemory(addr_start, addr_min, addr_max, &value, sizeof(value), backward, offset));
	}

	uint16_t* FindU16(const void* addr_start, const void* addr_min, const void* addr_max, uint16_t value, bool backward, ptrdiff_t offset)
	{
		return static_cast<uint16_t*>(FindMemory(addr_start, addr_min, addr_max, &value, sizeof(value), backward, offset));
	}

	uint32_t* FindU24(const void* addr_start, const void* addr_min, const void* addr_max, const uint8_t value[3], bool backward, ptrdiff_t offset)
	{
		return static_cast<uint32_t*>(FindMemory(addr_start, addr_min, addr_max, value, 3, backward, offset));
	}

	uint32_t* FindU24(const void* addr_start, const void* addr_min, const void* addr_max, uint32_t value, bool backward, ptrdiff_t offset)
	{
		uint8_t bytes[3] = { static_cast<uint8_t>(value & 0xFF), static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>((value >> 16) & 0xFF) };
		return static_cast<uint32_t*>(FindMemory(addr_start, addr_min, addr_max, bytes, sizeof(bytes), backward, offset));
	}

	uint32_t* FindU32(const void* addr_start, const void* addr_min, const void* addr_max, uint32_t value, bool backward, ptrdiff_t offset)
	{
		return static_cast<uint32_t*>(FindMemory(addr_start, addr_min, addr_max, &value, sizeof(value), backward, offset));
	}

	void* FindSignature(const void* addr_start, const void* addr_min, const void* addr_max, std::string_view signature, bool backward, ptrdiff_t offset)
	{
		std::vector<PatternByte> pattern;
		if (!ParseSignature(signature, pattern))
			return nullptr;

		if (!addr_start || !addr_min || !addr_max)
			return nullptr;

		auto lower = reinterpret_cast<uintptr_t>(addr_min);
		auto upper = reinterpret_cast<uintptr_t>(addr_max);
		if (lower >= upper || pattern.size() > upper - lower)
			return nullptr;

		auto first = lower;
		auto last = upper - pattern.size();
		auto current = std::clamp(reinterpret_cast<uintptr_t>(addr_start), first, last);

		for (;;)
		{
			if (MatchSignature(reinterpret_cast<const uint8_t*>(current), pattern))
				return reinterpret_cast<void*>(current + offset);

			if (backward)
			{
				if (current == first)
					break;
				--current;
			}
			else
			{
				if (current == last)
					break;
				++current;
			}
		}

		return nullptr;
	}

	bool WriteMemory(void* addr, const void* data, size_t size, ptrdiff_t offset, bool use_setmem)
	{
		if (!addr || !data || size == 0)
			return false;

		if (offset != 0)
			addr = PtrOffset(addr, offset);

		DWORD old_protection = 0;
		DWORD new_protection = IsMemoryExecutable(addr) ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
		if (!VirtualProtect(addr, size, new_protection, &old_protection))
			return false;

		if (use_setmem)
			std::memset(addr, static_cast<int>(*static_cast<const uint8_t*>(data)), size);
		else
			std::memcpy(addr, data, size);

		FlushInstructionCache(GetCurrentProcess(), addr, size);
		DWORD temporary = 0;
		return VirtualProtect(addr, size, old_protection, &temporary) != FALSE;
	}

	bool WriteU8(void* addr, uint8_t value, ptrdiff_t offset) { return WriteMemory(addr, &value, sizeof(value), offset); }
	bool WriteU16(void* addr, uint16_t value, ptrdiff_t offset) { return WriteMemory(addr, &value, sizeof(value), offset); }
	bool WriteU24(void* addr, const int8_t value[3], ptrdiff_t offset) { return WriteMemory(addr, value, 3, offset); }
	bool WriteU32(void* addr, uint32_t value, ptrdiff_t offset) { return WriteMemory(addr, &value, sizeof(value), offset); }
	bool WritePointer(void* addr, const void* value, ptrdiff_t offset) { return WriteMemory(addr, &value, sizeof(value), offset); }

	bool WriteU24(void* addr, uint32_t value, ptrdiff_t offset)
	{
		uint8_t bytes[3] = { static_cast<uint8_t>(value & 0xFF), static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>((value >> 16) & 0xFF) };
		return WriteMemory(addr, bytes, sizeof(bytes), offset);
	}

	static bool WriteBranch(void* addr, void* target, uint8_t opcode)
	{
		if (!addr || !target)
			return false;

		uint8_t instruction[5] = { opcode, 0, 0, 0, 0 };
		intptr_t difference = reinterpret_cast<intptr_t>(target) - (reinterpret_cast<intptr_t>(addr) + 5);
		int32_t displacement = static_cast<int32_t>(difference);
		std::memcpy(instruction + 1, &displacement, sizeof(displacement));
		return WriteMemory(addr, instruction, sizeof(instruction));
	}

	bool WriteCall(void* addr, void* target) { return WriteBranch(addr, target, 0xE8); }
	bool WriteJump(void* addr, void* target) { return WriteBranch(addr, target, 0xE9); }

	static bool CheckMemory(const void* addr, const void* value, size_t size, ptrdiff_t offset)
	{
		if (!addr || !value || size == 0)
			return false;
		return std::memcmp(PtrOffset(addr, offset), value, size) == 0;
	}

	bool CheckU8(const void* addr, uint8_t value, ptrdiff_t offset) { return CheckMemory(addr, &value, sizeof(value), offset); }
	bool CheckU16(const void* addr, uint16_t value, ptrdiff_t offset) { return CheckMemory(addr, &value, sizeof(value), offset); }
	bool CheckU32(const void* addr, uint32_t value, ptrdiff_t offset) { return CheckMemory(addr, &value, sizeof(value), offset); }
	bool CheckU24(const void* addr, const uint8_t value[3], ptrdiff_t offset) { return CheckMemory(addr, value, 3, offset); }

	bool CheckU24(const void* addr, uint32_t value, ptrdiff_t offset)
	{
		uint8_t bytes[3] = { static_cast<uint8_t>(value & 0xFF), static_cast<uint8_t>((value >> 8) & 0xFF), static_cast<uint8_t>((value >> 16) & 0xFF) };
		return CheckMemory(addr, bytes, sizeof(bytes), offset);
	}

	void* Advance(void* addr, ptrdiff_t offset, bool dereference)
	{
		return PtrOffset(addr, offset, dereference);
	}

	void* Resolve(void* addr, ptrdiff_t pre_offset, ptrdiff_t post_offset)
	{
		return addr ? Resolve(PtrOffset(addr, pre_offset), post_offset) : nullptr;
	}

	void* Resolve(void* addr, ptrdiff_t offset)
	{
		if (!addr)
			return nullptr;
		int32_t relative = 0;
		std::memcpy(&relative, addr, sizeof(relative));
		return PtrOffset(addr, static_cast<ptrdiff_t>(relative) + offset);
	}

	bool Splice(void* splice_addr, void* jump_to, void* out_tramp, bool place_call)
	{
		if (!splice_addr || !jump_to)
			return false;

		auto source = static_cast<uint8_t*>(splice_addr);
		auto destination = static_cast<uint8_t*>(jump_to);
		std::vector<RelocationInstruction> instructions;
		size_t patch_size = 0;
		size_t relocated_size = 0;
		if (!AnalyzeSpliceBlock(source, instructions, patch_size, relocated_size))
			return false;

		size_t trampoline_size = relocated_size + 5;
		size_t total_size = sizeof(TrampolineMeta) + trampoline_size + patch_size;
		auto base = static_cast<uint8_t*>(VirtualAlloc(nullptr, total_size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		if (!base)
			return false;

		auto meta = reinterpret_cast<TrampolineMeta*>(base);
		auto trampoline = base + sizeof(TrampolineMeta);
		auto original_copy = trampoline + trampoline_size;
		meta->HookSize = static_cast<uint32_t>(patch_size);
		meta->TrampolineSize = static_cast<uint32_t>(trampoline_size);
		meta->OriginalAddress = splice_addr;

		if (!RelocateInstructions(source, patch_size, trampoline, instructions))
		{
			VirtualFree(base, 0, MEM_RELEASE);
			return false;
		}

		auto jump_back = trampoline + relocated_size;
		jump_back[0] = 0xE9;
		WriteRel32(jump_back, source + patch_size, 5);
		std::memcpy(original_copy, source, patch_size);
		FlushInstructionCache(GetCurrentProcess(), trampoline, trampoline_size);

		DWORD old_protection = 0;
		if (!VirtualProtect(source, patch_size, PAGE_EXECUTE_READWRITE, &old_protection))
		{
			VirtualFree(base, 0, MEM_RELEASE);
			return false;
		}

		std::memset(source, 0x90, patch_size);
		source[0] = place_call ? 0xE8 : 0xE9;
		intptr_t difference = reinterpret_cast<intptr_t>(destination) - reinterpret_cast<intptr_t>(source + 5);
		int32_t displacement = static_cast<int32_t>(difference);
		std::memcpy(source + 1, &displacement, sizeof(displacement));
		FlushInstructionCache(GetCurrentProcess(), source, patch_size);

		DWORD temporary = 0;
		if (!VirtualProtect(source, patch_size, old_protection, &temporary))
		{
			std::memcpy(source, original_copy, patch_size);
			FlushInstructionCache(GetCurrentProcess(), source, patch_size);
			VirtualFree(base, 0, MEM_RELEASE);
			return false;
		}

		if (out_tramp)
			*reinterpret_cast<void**>(out_tramp) = trampoline;
		return true;
	}

	bool SpliceAPI(const char* modname, const char* funcname, void* jump_to, void* out_tramp, bool place_call)
	{
		HMODULE module = GetModuleHandleA(modname);
		if (!module)
			return false;
		FARPROC function = GetProcAddress(module, funcname);
		return function ? Splice(reinterpret_cast<void*>(function), jump_to, out_tramp, place_call) : false;
	}

	bool Unsplice(void* tramp)
	{
		if (!tramp)
			return false;

		auto trampoline = static_cast<uint8_t*>(tramp);
		auto meta = reinterpret_cast<TrampolineMeta*>(trampoline - sizeof(TrampolineMeta));
		if (!meta->OriginalAddress || meta->HookSize < 5 || meta->TrampolineSize < 5)
			return false;

		auto original = static_cast<uint8_t*>(meta->OriginalAddress);
		auto original_copy = trampoline + meta->TrampolineSize;
		DWORD old_protection = 0;
		if (!VirtualProtect(original, meta->HookSize, PAGE_EXECUTE_READWRITE, &old_protection))
			return false;

		std::memcpy(original, original_copy, meta->HookSize);
		FlushInstructionCache(GetCurrentProcess(), original, meta->HookSize);
		DWORD temporary = 0;
		VirtualProtect(original, meta->HookSize, old_protection, &temporary);
		VirtualFree(meta, 0, MEM_RELEASE);
		return true;
	}

	bool FillChar(void* addr, int value, size_t size)
	{
		uint8_t byte = static_cast<uint8_t>(value);
		return WriteMemory(addr, &byte, size, 0, true);
	}

	bool FillNops(void* addr, size_t size) { return FillChar(addr, 0x90, size); }
	bool IsDllLoaded(const char* name) { return GetModuleHandleA(name) != nullptr; }

	class MemoryBackup
	{
	public:
		MemoryBackup(const void* address, size_t size) : Address(const_cast<void*>(address)), Size(size), Data(size)
		{
			if (Address && Size != 0)
				std::memcpy(Data.data(), Address, Size);
		}

		bool Restore() const
		{
			return Address && Size != 0 && !Data.empty() && WriteMemory(Address, Data.data(), Size);
		}

	private:
		void* Address = nullptr;
		size_t Size = 0;
		std::vector<uint8_t> Data;
	};

	static constexpr size_t MaxBackups = 64;
	static std::vector<MemoryBackup> gBackups;

	bool CreateMemoryBackup(const void* address, size_t size)
	{
		if (!address || size == 0 || gBackups.size() >= MaxBackups)
			return false;
		gBackups.emplace_back(address, size);
		return true;
	}

	bool RestoreMemoryBackups()
	{
		if (gBackups.empty())
			return false;

		bool result = true;
		for (auto it = gBackups.rbegin(); it != gBackups.rend(); ++it)
			result = it->Restore() && result;
		gBackups.clear();
		return result;
	}
}
