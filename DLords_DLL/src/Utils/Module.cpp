#include "Module.hpp"
#include "Memory.hpp"

#include <algorithm>

namespace U::Memory
{
	static DWORD GetModuleSize(HMODULE handle)
	{
		if (!handle)
			return 0;

		auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(handle);
		if (dos->e_magic != IMAGE_DOS_SIGNATURE)
			return 0;

		auto nt = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(handle) + dos->e_lfanew);
		if (nt->Signature != IMAGE_NT_SIGNATURE)
			return 0;
		return nt->OptionalHeader.SizeOfImage;
	}

	sighandle_t::sighandle_t(module_t* module) : Module(module), Ptr(module ? module->Handle : nullptr) {}

	bool sighandle_t::IsValid() const
	{
		return Module && Ptr && Module->Handle && Module->LastByte && IsInBounds(Ptr, Module->Handle, Module->LastByte);
	}

	void* sighandle_t::Get() const { return Ptr; }

	sighandle_t& sighandle_t::Reset()
	{
		Ptr = Module ? Module->Handle : nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::Invalidate()
	{
		Ptr = nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::FindU8(uint8_t value, bool backward, ptrdiff_t offset)
	{
		Ptr = Module ? U::Memory::FindU8(Ptr, Module->Handle, Module->LastByte, value, backward, offset) : nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::FindU16(uint16_t value, bool backward, ptrdiff_t offset)
	{
		Ptr = Module ? U::Memory::FindU16(Ptr, Module->Handle, Module->LastByte, value, backward, offset) : nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::FindU24(const uint8_t value[3], bool backward, ptrdiff_t offset)
	{
		Ptr = Module ? U::Memory::FindU24(Ptr, Module->Handle, Module->LastByte, value, backward, offset) : nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::FindU24(uint32_t value, bool backward, ptrdiff_t offset)
	{
		Ptr = Module ? U::Memory::FindU24(Ptr, Module->Handle, Module->LastByte, value, backward, offset) : nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::FindU32(uint32_t value, bool backward, ptrdiff_t offset)
	{
		Ptr = Module ? U::Memory::FindU32(Ptr, Module->Handle, Module->LastByte, value, backward, offset) : nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::FindSignature(std::string_view signature, bool backward, ptrdiff_t offset)
	{
		Ptr = Module ? U::Memory::FindSignature(Ptr, Module->Handle, Module->LastByte, signature, backward, offset) : nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::Advance(ptrdiff_t offset)
	{
		Ptr = U::Memory::Advance(Ptr, offset);
		return *this;
	}

	sighandle_t& sighandle_t::Add(size_t value) { return Advance(static_cast<ptrdiff_t>(value)); }
	sighandle_t& sighandle_t::Sub(size_t value) { return Advance(-static_cast<ptrdiff_t>(value)); }

	sighandle_t& sighandle_t::Dereference()
	{
		Ptr = Ptr ? *reinterpret_cast<void**>(Ptr) : nullptr;
		return *this;
	}

	sighandle_t& sighandle_t::Resolve()
	{
		Ptr = U::Memory::Resolve(Ptr, 0, sizeof(uint32_t));
		return *this;
	}

	sighandle_t& sighandle_t::Resolve(ptrdiff_t offset)
	{
		Ptr = U::Memory::Resolve(Ptr, offset);
		return *this;
	}

	sighandle_t& sighandle_t::Resolve(ptrdiff_t pre_offset, ptrdiff_t post_offset)
	{
		Ptr = U::Memory::Resolve(Ptr, pre_offset, post_offset);
		return *this;
	}

	sighandle_t& sighandle_t::Rip() { return Dereference(); }

	bool sighandle_t::CheckU8(uint8_t value, ptrdiff_t offset) const { return U::Memory::CheckU8(Ptr, value, offset); }
	bool sighandle_t::CheckU16(uint16_t value, ptrdiff_t offset) const { return U::Memory::CheckU16(Ptr, value, offset); }
	bool sighandle_t::CheckU24(uint32_t value, ptrdiff_t offset) const { return U::Memory::CheckU24(Ptr, value, offset); }
	bool sighandle_t::CheckU24(const uint8_t value[3], ptrdiff_t offset) const { return U::Memory::CheckU24(Ptr, value, offset); }
	bool sighandle_t::CheckU32(uint32_t value, ptrdiff_t offset) const { return U::Memory::CheckU32(Ptr, value, offset); }

	module_t::module_t(const char* libname)
	{
		if (!libname)
			return;
		Name = libname;
		Handle = GetModuleHandleA(libname);
		Size = GetModuleSize(Handle);
		LastByte = Handle && Size != 0 ? PtrOffset(Handle, Size) : nullptr;
	}

	bool module_t::GetLoaded() const { return Handle != nullptr && Size != 0; }

	std::string module_t::GetName(bool no_extension) const
	{
		if (!no_extension)
			return Name;
		auto separator = Name.find_last_of("/\\");
		auto start = separator == std::string::npos ? 0 : separator + 1;
		auto dot = Name.find_last_of('.');
		if (dot == std::string::npos || dot < start)
			return Name.substr(start);
		return Name.substr(start, dot - start);
	}

	void* module_t::GetBase() const { return Handle; }
	size_t module_t::GetSize() const { return Size; }
	void* module_t::GetLastByte() const { return LastByte; }
	sighandle_t module_t::Sig() { return sighandle_t(this); }
}
