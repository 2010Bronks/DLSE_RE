#pragma once

#include <Windows.h>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace U::Memory
{
	struct module_t;

	struct sighandle_t
	{
		module_t* Module = nullptr;
		void* Ptr = nullptr;

		explicit sighandle_t(module_t* module);

		bool IsValid() const;
		void* Get() const;

		sighandle_t& Reset();
		sighandle_t& Invalidate();
		sighandle_t& FindU8(uint8_t value, bool backward = false, ptrdiff_t offset = 0);
		sighandle_t& FindU16(uint16_t value, bool backward = false, ptrdiff_t offset = 0);
		sighandle_t& FindU24(const uint8_t value[3], bool backward = false, ptrdiff_t offset = 0);
		sighandle_t& FindU24(uint32_t value, bool backward = false, ptrdiff_t offset = 0);
		sighandle_t& FindU32(uint32_t value, bool backward = false, ptrdiff_t offset = 0);
		sighandle_t& FindSignature(std::string_view signature, bool backward = false, ptrdiff_t offset = 0);
		sighandle_t& Advance(ptrdiff_t offset);
		sighandle_t& Add(size_t value);
		sighandle_t& Sub(size_t value);
		sighandle_t& Dereference();
		sighandle_t& Resolve();
		sighandle_t& Resolve(ptrdiff_t offset);
		sighandle_t& Resolve(ptrdiff_t pre_offset, ptrdiff_t post_offset);
		sighandle_t& Rip();

		bool CheckU8(uint8_t value, ptrdiff_t offset = 0) const;
		bool CheckU16(uint16_t value, ptrdiff_t offset = 0) const;
		bool CheckU24(uint32_t value, ptrdiff_t offset = 0) const;
		bool CheckU24(const uint8_t value[3], ptrdiff_t offset = 0) const;
		bool CheckU32(uint32_t value, ptrdiff_t offset = 0) const;
	};

	struct module_t
	{
		HMODULE Handle = nullptr;
		DWORD Size = 0;
		void* LastByte = nullptr;
		std::string Name;

		module_t() = default;
		explicit module_t(const char* libname);

		bool GetLoaded() const;
		std::string GetName(bool no_extension = true) const;
		void* GetBase() const;
		size_t GetSize() const;
		void* GetLastByte() const;
		sighandle_t Sig();
	};
}
