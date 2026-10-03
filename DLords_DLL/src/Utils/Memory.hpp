#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace U::Memory
{
	void* PtrOffset(const void* addr, ptrdiff_t offset, bool dereference = false);

	uint8_t* FindU8(const void* addr_start, const void* addr_min, const void* addr_max, uint8_t value, bool backward = false, ptrdiff_t offset = 0);
	uint16_t* FindU16(const void* addr_start, const void* addr_min, const void* addr_max, uint16_t value, bool backward = false, ptrdiff_t offset = 0);
	uint32_t* FindU24(const void* addr_start, const void* addr_min, const void* addr_max, const uint8_t value[3], bool backward = false, ptrdiff_t offset = 0);
	uint32_t* FindU24(const void* addr_start, const void* addr_min, const void* addr_max, uint32_t value, bool backward = false, ptrdiff_t offset = 0);
	uint32_t* FindU32(const void* addr_start, const void* addr_min, const void* addr_max, uint32_t value, bool backward = false, ptrdiff_t offset = 0);
	void* FindSignature(const void* addr_start, const void* addr_min, const void* addr_max, std::string_view signature, bool backward = false, ptrdiff_t offset = 0);

	bool WriteMemory(void* addr, const void* data, size_t size, ptrdiff_t offset = 0, bool use_setmem = false);
	bool WriteU8(void* addr, uint8_t value, ptrdiff_t offset = 0);
	bool WriteU16(void* addr, uint16_t value, ptrdiff_t offset = 0);
	bool WriteU24(void* addr, uint32_t value, ptrdiff_t offset = 0);
	bool WriteU24(void* addr, const int8_t value[3], ptrdiff_t offset = 0);
	bool WriteU32(void* addr, uint32_t value, ptrdiff_t offset = 0);
	bool WritePointer(void* addr, const void* value, ptrdiff_t offset = 0);
	bool WriteCall(void* addr, void* target);
	bool WriteJump(void* addr, void* target);

	bool CheckU8(const void* addr, uint8_t value, ptrdiff_t offset = 0);
	bool CheckU16(const void* addr, uint16_t value, ptrdiff_t offset = 0);
	bool CheckU24(const void* addr, uint32_t value, ptrdiff_t offset = 0);
	bool CheckU24(const void* addr, const uint8_t value[3], ptrdiff_t offset = 0);
	bool CheckU32(const void* addr, uint32_t value, ptrdiff_t offset = 0);

	void* Advance(void* addr, ptrdiff_t offset, bool dereference = false);
	void* Resolve(void* addr, ptrdiff_t pre_offset, ptrdiff_t post_offset);
	void* Resolve(void* addr, ptrdiff_t offset = 0);

	bool Splice(void* splice_addr, void* jump_to, void* out_tramp, bool place_call = false);
	bool SpliceAPI(const char* modname, const char* funcname, void* jump_to, void* out_tramp, bool place_call = false);
	bool Unsplice(void* tramp);

	bool IsInBounds(uintptr_t addr, uintptr_t addr_lower, uintptr_t addr_upper);
	bool IsInBounds(const void* addr, const void* addr_lower, const void* addr_upper);
	bool FillChar(void* addr, int value, size_t size);
	bool FillNops(void* addr, size_t size);
	bool IsDllLoaded(const char* name);

	bool CreateMemoryBackup(const void* address, size_t size);
	bool RestoreMemoryBackups();
}
