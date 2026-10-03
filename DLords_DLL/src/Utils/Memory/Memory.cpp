#include "precompiled.hpp"

#include <list>
#include <memory>
#include <filesystem>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

// Мини-менеджмент для работы с аллоцированной памятью Memoria.
#pragma region Memory Chunk
struct MemoryChunk
{
public:
	void* _chunk;

public:
	MemoryChunk()
	{
		_chunk = {};
	}

	MemoryChunk(void* chunk) : _chunk(chunk)
	{
		assert(chunk);

		_chunk = chunk;
	}

	~MemoryChunk()
	{
		if (_chunk)
			Memoria::FreeMemory(_chunk);
	}
};

static std::list<MemoryChunk> AllocatedChunks{};
#pragma endregion

// Стек протекции памяти для PushMemoryProtection/PopMemoryProtection.
static std::stack<std::pair<size_t, DWORD>> ProtectionStack{};

static std::list<std::unique_ptr<DetourInfo>> HookInfos{};
static std::list<std::unique_ptr<RefDetourInfo>> RefHookInfos{};

static std::list<Memoria::CMemoryPatch> MemoryPatches{};

static DWORD CreateVirtualFlags(bool bExecutable, bool bReadable, bool bWritable)
{
	DWORD flags{};

	if (bExecutable)
	{
		if (bReadable && bWritable)
		{
			flags = PAGE_EXECUTE_READWRITE;
		}
		else if (bReadable && !bWritable)
		{
			flags = PAGE_EXECUTE_READ;
		}
		else
		{
			flags = PAGE_EXECUTE;
		}
	}
	else
	{
		if (bReadable && bWritable)
		{
			flags = PAGE_READWRITE;
		}
		else if (bReadable && !bWritable)
		{
			flags = PAGE_READONLY;
		}
		else
		{
			flags = PAGE_NOACCESS;
		}
	}

	return flags;
}

void* Memoria::AllocMemory(size_t nSize, bool bExecutable, bool bReadable, bool bWritable)
{
	if (nSize == 0)
		return nullptr;

	void* result = VirtualAlloc(nullptr, nSize, MEM_COMMIT, CreateVirtualFlags(bExecutable, bReadable, bWritable));
	if (!result)
		return nullptr;

	AllocatedChunks.emplace_front(result);
	return result;
}

bool Memoria::FreeMemory(void* pMemory)
{
	return VirtualFree(pMemory, 0, MEM_RELEASE) != FALSE;
}

void* Memoria::Resolve(void* pAddr, ptrdiff_t nPreOffset, ptrdiff_t nPostOffset)
{
	return Memoria::Resolve(reinterpret_cast<void*>(ptrdiff_t(pAddr) + nPreOffset), nPostOffset);
}

void* Memoria::Resolve(void* pAddr, ptrdiff_t nOffset)
{
	return Advance(pAddr, *static_cast<int*>(pAddr) + nOffset);
}

int32_t Memoria::Relative(void* pBase, void* pAddr, size_t nInstrSize)
{
	return reinterpret_cast<int32_t>(Advance(pBase, -reinterpret_cast<intptr_t>(pAddr) - nInstrSize));
}

void* Memoria::Advance(void* pAddr, ptrdiff_t nOffset, bool bDereference)
{
	void* result = reinterpret_cast<void*>(reinterpret_cast<intptr_t>(pAddr) + nOffset);

	if (bDereference)
		result = *reinterpret_cast<void**>(result);

	return result;
}

bool Memoria::IsWithinBounds(uintptr_t pAddress, uintptr_t pLowerBound, uintptr_t pUpperBound)
{
	if (pAddress < pLowerBound)
		return false;

	if (pAddress >= pUpperBound)
		return false;

	return true;
}

bool Memoria::IsWithinBounds(void* pAddress, void* pLowerBound, void* pUpperBound)
{
	auto addr = reinterpret_cast<uintptr_t>(pAddress);
	auto lower = reinterpret_cast<uintptr_t>(pLowerBound);
	auto upper = reinterpret_cast<uintptr_t>(pUpperBound);

	return IsWithinBounds(addr, lower, upper);
}

bool Memoria::IsMemoryValid(void* pAddress)
{
	MEMORY_BASIC_INFORMATION memInfo;

	if (VirtualQuery(pAddress, &memInfo, sizeof(memInfo)) == 0)
		return false;

	return !(memInfo.Protect == 0 || memInfo.Protect == PAGE_NOACCESS);
}

bool Memoria::IsMemoryExecutable(void* pAddress)
{
	MEMORY_BASIC_INFORMATION memInfo;

	if (VirtualQuery(pAddress, &memInfo, sizeof(memInfo)) == 0)
		return false;

	if (memInfo.Protect == 0 || memInfo.Protect == PAGE_NOACCESS)
		return false;

	return memInfo.Protect == PAGE_EXECUTE ||
		memInfo.Protect == PAGE_EXECUTE_READ ||
		memInfo.Protect == PAGE_EXECUTE_READWRITE ||
		memInfo.Protect == PAGE_EXECUTE_WRITECOPY;
}

// TODO: Get rid of it ASAP
static DWORD GetVirtualProtect(void* pAddress, size_t nSize)
{
	DWORD oldProtection, oldProtection2;

	if (!VirtualProtect(pAddress, nSize, PAGE_EXECUTE_READWRITE, &oldProtection))
		return PAGE_NOACCESS;

	if (!VirtualProtect(pAddress, nSize, oldProtection, &oldProtection2))
		return PAGE_NOACCESS;

	return oldProtection;
}

static bool PushMemoryProtection(void* pAddress, size_t nSize, const std::optional<DWORD>& nNewProtection)
{
	DWORD oldProtect;

	if (nNewProtection.has_value())
	{
		if (!VirtualProtect(pAddress, nSize, nNewProtection.value(), &oldProtect))
		{
			assert(false);
			return false;
		}
	}
	else
	{
		oldProtect = GetVirtualProtect(pAddress, nSize);

		if (oldProtect == PAGE_NOACCESS)
		{
			assert(false);
			return false;
		}
	}

	ProtectionStack.emplace(nSize, oldProtect);
	return true;
}

bool Memoria::PushMemoryProtection(void* pAddress, size_t nSize, DWORD nNewProtection)
{
	return ::PushMemoryProtection(pAddress, nSize, nNewProtection);
}

bool Memoria::PushMemoryProtection(void* pAddress, size_t nSize)
{
	return ::PushMemoryProtection(pAddress, nSize, std::nullopt);
}

bool Memoria::PopMemoryProtection(void* pAddress)
{
	if (ProtectionStack.empty())
	{
		assert(false);
		return false;
	}

	DWORD oldProtection;
	MEMORY_BASIC_INFORMATION mbi;

	if (VirtualQuery(pAddress, &mbi, sizeof(mbi)) == 0)
	{
		assert(false);
		return false;
	}

	auto&& [size, protection] = ProtectionStack.top();

	if (!VirtualProtect(pAddress, size, protection, &oldProtection))
	{
		assert(false);
		return false;
	}

	ProtectionStack.pop();
	return true;
}

bool Memoria::PopMemoryProtection()
{
	if (ProtectionStack.empty())
		return false;

	ProtectionStack.pop();
	return true;
}

void* Memoria::GetBaseAddress(void* pAddress)
{
	HMODULE result;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCSTR>(pAddress), &result))
	{
		return nullptr;
	}

	return result;
}

Memoria::CMemoryPatch* Memoria::GetPatchInfo(size_t nIndex)
{
	if (MemoryPatches.empty())
		return nullptr;

	if (nIndex > MemoryPatches.size())
		return nullptr;

	auto it = MemoryPatches.begin();
	std::advance(it, nIndex);
	return &*it;
}

void* Memoria::GetRelativeAddress(void* pAddress)
{
	void* baseAddr = GetBaseAddress(pAddress);

	if (baseAddr == 0)
		return pAddress;

	return reinterpret_cast<void*>(reinterpret_cast<size_t>(pAddress) - reinterpret_cast<size_t>(baseAddr));
}

DWORD Memoria::GetModuleSize(HMODULE hInstance)
{
	PIMAGE_DOS_HEADER dos = reinterpret_cast<PIMAGE_DOS_HEADER>(hInstance);
	PIMAGE_NT_HEADERS nt = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);

	return nt->OptionalHeader.SizeOfImage;
}

typedef struct _PEB_LDR_DATA
{
	UINT8 _PADDING_[12];
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

typedef struct _PEB
{
#ifdef _WIN64
	UINT8 _PADDING_[24];
#else
	UINT8 _PADDING_[12];
#endif
	PEB_LDR_DATA* Ldr;
} PEB, * PPEB;

struct UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
};

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

HMODULE Memoria::GetModuleHandleDirect(uint64_t module_name_hash)
{
#ifdef _WIN64
	PPEB pPeb = (PPEB)__readgsqword(0x60);
#elif _WIN32
	PPEB pPeb = (PPEB)__readfsdword(0x30);
#else
	assert(false);
	return 0;
#endif

	PLIST_ENTRY pListHead = &(pPeb->Ldr->InMemoryOrderModuleList);
	PLIST_ENTRY pListEntry = pListHead->Flink;

	while (pListEntry != pListHead)
	{
		PLDR_DATA_TABLE_ENTRY pModule = CONTAINING_RECORD(pListEntry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (FNV1a(pModule->BaseDllName.Buffer) == module_name_hash)
		{
			return (HMODULE)pModule->DllBase;
		}

		pListEntry = pListEntry->Flink;
	}

	return NULL;
}

void* Memoria::GetProcAddressDirect(uint64_t module_name_hash, uint64_t function_name_hash)
{
	HMODULE hModule = GetModuleHandleDirect(module_name_hash);
	if (!hModule)
		return NULL;

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((PBYTE)hModule + dosHeader->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY exportDir = (PIMAGE_EXPORT_DIRECTORY)((PBYTE)hModule + ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	DWORD exportSize = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;

	if (!exportDir || exportSize == 0)
		return NULL;

	PDWORD functions = (PDWORD)((PBYTE)hModule + exportDir->AddressOfFunctions);
	PDWORD names = (PDWORD)((PBYTE)hModule + exportDir->AddressOfNames);
	PWORD ordinals = (PWORD)((PBYTE)hModule + exportDir->AddressOfNameOrdinals);

	for (DWORD i = 0; i < exportDir->NumberOfNames; i++)
	{
		char* function_name = (char*)(reinterpret_cast<uintptr_t>(hModule) + names[i]);

		if (FNV1a(function_name) == function_name_hash)
			return (void*)((PBYTE)hModule + functions[ordinals[i]]);
	}

	return NULL;
}

bool Memoria::GetMemoryBlock(void* pSource, size_t nCount, void* pDest)
{
	if (pSource == nullptr || pDest == nullptr)
		return false;

	std::memcpy(pDest, pSource, nCount);
	return true;
}

void* Memoria::GetInterfaceAddress(HMODULE hModule, const std::string& sInterfaceName)
{
	using CreateInterfaceFunc = void* (*)(const char* name, int* returnCode);

	if (!hModule || sInterfaceName.empty())
		return nullptr;

	auto pfnGetInterface = reinterpret_cast<CreateInterfaceFunc>(GetProcAddress(hModule, "CreateInterface"));
	if (!pfnGetInterface)
		return nullptr;

	return pfnGetInterface(sInterfaceName.c_str(), nullptr);
}

void* Memoria::GetInterfaceAddress(const std::string& sModuleName, const std::string& sInterfaceName)
{
	using CreateInterfaceFunc = void* (*)(const char* name, int* returnCode);

	if (sModuleName.empty() || sInterfaceName.empty())
		return nullptr;

	auto hModule = GetModuleHandleA(sModuleName.c_str());
	if (!hModule)
		return nullptr;

	return GetInterfaceAddress(hModule, sInterfaceName);
}

std::span<uint8_t> Memoria::GetMemorySpan(void* pSource, size_t nCount)
{
	return std::span<uint8_t>(static_cast<uint8_t*>(pSource), nCount);
}

std::vector<uint8_t> Memoria::GetMemoryData(void* pSource, size_t nCount)
{
	std::vector<uint8_t> data(nCount);

	if (pSource != nullptr)
		std::memcpy(data.data(), pSource, nCount);

	return data;
}

bool Memoria::SetMemoryBlock(void* pDest, const void* pSource, size_t nCount, bool bModifyProtect)
{
	if (pDest == nullptr || pSource == nullptr || nCount == 0)
		return false;

	MemoryPatches.emplace_front(pDest, pSource, nCount, bModifyProtect);
	MemoryPatches.front().Apply();

	return true;
}

bool Memoria::SetMemorySpan(void* pDest, const std::span<const uint8_t>& data, bool bModifyProtect)
{
	if (pDest == nullptr || data.empty())
		return false;

	return Memoria::SetMemoryBlock(pDest, data.data(), data.size(), bModifyProtect);
}

bool Memoria::SetMemoryData(void* pDest, const std::vector<uint8_t>& data, bool bModifyProtect)
{
	if (pDest == nullptr || data.empty())
		return false;

	return Memoria::SetMemoryBlock(pDest, data.data(), data.size(), bModifyProtect);
}

static thread_local void* s_last_protection_address = nullptr;
static thread_local DWORD s_last_protection_value = 0;

DWORD Memoria::SetProtection(void* pCode, DWORD value)
{
	DWORD oldProtect;

	if (!VirtualProtect(pCode, 0x1000, value, &oldProtect))
		return 0;

	s_last_protection_address = pCode;
	s_last_protection_value = oldProtect;

	return oldProtect;
}

bool Memoria::RestoreLastProtection()
{
	bool result = false;

	if (s_last_protection_address && s_last_protection_value)
	{
		DWORD oldProtect;

		if (VirtualProtect(s_last_protection_address, 0x1000, s_last_protection_value, &oldProtect))
		{
			s_last_protection_address = nullptr;
			s_last_protection_value = 0;
			result = true;
		}
	}

	return result;
}

template <typename T>
static T* FindPrimitive(void* pStart, void* pLowerBound, void* pUpperBound, T aValue, bool back, int nOffset = 0)
{
	assert(pLowerBound != nullptr && pUpperBound != nullptr && pLowerBound <= pUpperBound);

	pUpperBound = reinterpret_cast<T*>(reinterpret_cast<intptr_t>(pUpperBound) - sizeof(T));

	T* result = (T*)pStart;

	do
	{
		if (!Memoria::IsWithinBounds(result, pLowerBound, pUpperBound))
		{
			return nullptr;
		}

		if (*result == aValue) {
			return static_cast<T*>(Memoria::Advance(result, nOffset));
		}

		if (back) {
			result = reinterpret_cast<T*>(uintptr_t(result) - 1);
		}
		else {
			result = reinterpret_cast<T*>(uintptr_t(result) + 1);
		}

	} while (true);

	return nullptr;
}

template <typename T>
static T* FindPrimitive(void* pStart, int aLimit, T aValue, bool back, int nOffset = 0)
{
	void* pLowerBound = Memoria::Advance(pStart, -aLimit);
	void* pUpperBound = Memoria::Advance(pStart, aLimit);

	void* result = static_cast<void*>(FindPrimitive(pStart, pLowerBound, pUpperBound, aValue, back));

	if (result != nullptr && nOffset != 0)
		result = Memoria::Advance(result, nOffset);

	return static_cast<T*>(result);
}

template <typename T>
static void WritePrimitive(void* aAddr, T aValue, bool bModifyProtect = true)
{
	assert(aAddr != nullptr);

	Memoria::SetMemoryBlock(aAddr, &aValue, sizeof(aValue), bModifyProtect);
}

template <typename T>
static bool CheckPrimitive(void* aAddr, T aValue, int aOffset)
{
	assert(aAddr != nullptr);

	if (!aAddr)
		return false;

	aAddr = Memoria::Advance(aAddr, aOffset);
	return std::memcmp(aAddr, &aValue, sizeof(aValue)) == 0;
}

uint8_t* Memoria::FindUInt8(void* aStart, int aLimit, uint8_t aValue, bool bBackward, int nOffset)
{
	return FindPrimitive(aStart, aLimit, aValue, bBackward, nOffset);
}

uint16_t* Memoria::FindUInt16(void* aStart, int aLimit, uint16_t aValue, bool bBackward, int nOffset)
{
	return FindPrimitive(aStart, aLimit, aValue, bBackward, nOffset);
}

uint32_t* Memoria::FindUInt32(void* aStart, int aLimit, uint32_t aValue, bool bBackward, int nOffset)
{
	return FindPrimitive(aStart, aLimit, aValue, bBackward, nOffset);
}

uint64_t* Memoria::FindUInt64(void* aStart, int aLimit, uint64_t aValue, bool bBackward, int nOffset)
{
	return FindPrimitive(aStart, aLimit, aValue, bBackward, nOffset);
}

uint8_t* Memoria::FindUInt8(void* pStart, void* pLowerBound, void* pUpperBound, uint8_t aValue, bool bBackward, int nOffset)
{
	return FindPrimitive(pStart, pLowerBound, pUpperBound, aValue, bBackward, nOffset);
}

uint16_t* Memoria::FindUInt16(void* pStart, void* pLowerBound, void* pUpperBound, uint16_t aValue, bool bBackward, int nOffset)
{
	return FindPrimitive(pStart, pLowerBound, pUpperBound, aValue, bBackward, nOffset);
}

uint32_t* Memoria::FindUInt32(void* pStart, void* pLowerBound, void* pUpperBound, uint32_t aValue, bool bBackward, int nOffset)
{
	return FindPrimitive(pStart, pLowerBound, pUpperBound, aValue, bBackward, nOffset);
}

uint64_t* Memoria::FindUInt64(void* pStart, void* pLowerBound, void* pUpperBound, uint64_t aValue, bool bBackward, int nOffset)
{
	return FindPrimitive(pStart, pLowerBound, pUpperBound, aValue, bBackward, nOffset);
}

void* Memoria::FindSignature(void* pStart, void* pLowerBound, void* pUpperBound, const std::string& signature, bool bBackward, int nOffset)
{
	assert(pLowerBound != nullptr && pUpperBound != nullptr && !signature.empty());

	if (pLowerBound == nullptr || pUpperBound == nullptr || signature.empty())
		return nullptr;

	return Memoria::FindBlock(pStart, pLowerBound, pUpperBound, CreateStdSignature(signature), bBackward, nOffset);
}

void* Memoria::FindBlock(void* pStart, void* pLowerBound, void* pUpperBound, const void* aData, size_t aSize, bool bBackward, int nOffset)
{
	assert(pLowerBound != nullptr && pUpperBound != nullptr && aData != nullptr && aSize != 0);

	if (pLowerBound == nullptr || pUpperBound == nullptr || aData == nullptr || aSize == 0)
		return nullptr;

	uint8_t* result = (uint8_t*)pStart;

	pUpperBound = Memoria::Advance(pUpperBound, -(signed)aSize);

	do
	{
		if (!Memoria::IsWithinBounds(result, pLowerBound, pUpperBound))
		{
			return nullptr;
		}

		uint8_t* pData = (uint8_t*)aData;

		if (*pData == *result)
		{
			if (!memcmp(pData, result, aSize))
				return reinterpret_cast<void*>(ptrdiff_t(result) + nOffset);
		}

		if (bBackward)
			--result;
		else
			++result;
	} while (true);

	return nullptr;
}

void* Memoria::FindBlock(void* pStart, void* pLowerBound, void* pUpperBound, const std::vector<std::optional<uint8_t>>& aValue, bool bBackward, int nOffset)
{
	assert(pLowerBound != nullptr && pUpperBound != nullptr && !aValue.empty());
	assert(!aValue.empty() && aValue[0] != std::nullopt);

	if (pLowerBound == nullptr || pUpperBound == nullptr)
		return nullptr;

	if (aValue.empty() || aValue[0] == std::nullopt)
		return nullptr;

	uint8_t* result = (uint8_t*)pStart;

	pUpperBound = Memoria::Advance(pUpperBound, -(signed)aValue.size());

	do
	{
		if (!Memoria::IsWithinBounds(result, pLowerBound, pUpperBound))
			return nullptr;

		if (*result == aValue[0])
		{
			bool bFailed = false;
			uint8_t* p = result;

			for (auto i = 0u; i < aValue.size(); i++, p++)
			{
				if (aValue[i] == std::nullopt)
					continue;

				if (*p != aValue[i])
				{
					bFailed = true;
					break;
				}
			}

			if (!bFailed)
				return Memoria::Advance(result, nOffset);
		}

		if (bBackward)
			--result;
		else
			++result;
	} while (true);

	return nullptr;
}

void* Memoria::FindRelative(void* pStart, void* pLowerBound, void* pUpperBound, uint16_t nOpcode, int nIndex, bool bBack, int nOffset)
{
	void* result = pStart;

	do
	{
		result = FindReference(result, pLowerBound, pUpperBound, nullptr, nOpcode, bBack);

		if (result == nullptr)
			return nullptr;

		if (nIndex <= 0)
			return Memoria::Advance(result, nOffset);

		--nIndex;
		result = Memoria::Advance(result, (bBack) ? (-1) : (1));
	} while (true);
}

void* Memoria::FindReference(void* pStart, void* pLowerBound, void* pUpperBound, void* aRefAddr, uint16_t nOpcode, bool bBack, int nOffset)
{
	assert(pLowerBound != nullptr && pUpperBound != nullptr && pStart != nullptr);

	bool isTwoBytes = (nOpcode > 255), isOpcode = (nOpcode != 0);
	void* f = nullptr;

	auto Transfer = [](void* addr, bool back) -> void* {
		if (back) {
			return reinterpret_cast<void*>(reinterpret_cast<char*>(addr) - 1);
		}
		else {
			return reinterpret_cast<void*>(reinterpret_cast<char*>(addr) + 1);
		}
		};

	auto IsAbsoluteOpcode = [](unsigned short aOpcode) -> bool
		{
			return aOpcode != 0x68;
		};


	pUpperBound = Memoria::Advance(pUpperBound, -(signed)sizeof(void*));
	void* result = pStart;

	do
	{
		if (!Memoria::IsWithinBounds(result, pLowerBound, pUpperBound))
		{
			return nullptr;
		}

		if (isOpcode)
		{
			if (isTwoBytes)
			{
				result = FindPrimitive<unsigned short>(result, pLowerBound, pUpperBound, nOpcode, bBack);
			}
			else
			{
				void* r;
				r = FindPrimitive<unsigned char>(result, pLowerBound, pUpperBound, static_cast<uint8_t>(nOpcode), bBack);
				result = r;
			}

			if (result == nullptr)
			{
				return nullptr;
			}

			f = result;

			if (isTwoBytes)
			{
				f = Memoria::Advance(f, 2);
			}
			else {
				f = Memoria::Advance(f, 1);
			}

			if (IsAbsoluteOpcode(nOpcode))
			{
				f = Resolve(f, 4);
			}
			else
			{
				f = *static_cast<void**>(f);
			}
		}
		else {
			f = *static_cast<void**>(result);
		}

		if (!Memoria::IsWithinBounds(f, pLowerBound, pUpperBound))
		{
			result = Transfer(result, bBack);
			continue;
		}

		if (aRefAddr == nullptr || aRefAddr == f) {
			return Memoria::Advance(result, nOffset);
		}

		result = Transfer(result, bBack);
	} while (true);

	return nullptr;
}

void* Memoria::FindAnsiString(void* pStart, void* pLowerBound, void* pUpperBound, const char* aData, int nOffset)
{
	assert(pLowerBound != nullptr && pUpperBound != nullptr && aData != nullptr && strlen(aData) > 0);

	return Memoria::FindBlock(pStart, pLowerBound, pUpperBound, (void*)aData, strlen(aData), false, nOffset);
}

void Memoria::WriteRelative(void* pAddr, void* pValue, bool bModifyProtect)
{
	assert((pAddr != nullptr) && (pValue != nullptr));

	WritePrimitive(pAddr, Relative(pValue, pAddr), bModifyProtect);
}

void Memoria::WriteUInt8(void* pAddr, uint8_t nValue, bool bModifyProtect)
{
	WritePrimitive(pAddr, nValue, bModifyProtect);
}

void Memoria::WriteUInt16(void* pAddr, uint16_t nValue, bool bModifyProtect)
{
	WritePrimitive(pAddr, nValue, bModifyProtect);
}

void Memoria::WriteUInt32(void* pAddr, uint32_t nValue, bool bModifyProtect)
{
	WritePrimitive(pAddr, nValue, bModifyProtect);
}

void Memoria::WriteUInt64(void* pAddr, uint64_t nValue, bool bModifyProtect)
{
	WritePrimitive(pAddr, nValue, bModifyProtect);
}

void Memoria::WritePointer(void* pAddr, const void* pValue, bool bModifyProtect)
{
	WritePrimitive(pAddr, pValue, bModifyProtect);
}

void Memoria::FillChar(void* pAddr, unsigned char cValue, size_t nSize, bool bModifyProtect)
{
	assert(pAddr && nSize > 0);

	if (!pAddr || nSize == 0)
		return;

	std::vector<unsigned char> vPayload(nSize, cValue);
	SetMemoryBlock(pAddr, vPayload.data(), vPayload.size(), bModifyProtect);
}

void Memoria::FillNops(void* pAddr, size_t nSize, bool bModifyProtect)
{
	assert(pAddr && nSize > 0);

	if (!pAddr || nSize == 0)
		return;

	return Memoria::FillChar(pAddr, 0x90, nSize, bModifyProtect);
}

void Memoria::WriteFunc(void* pAddr, void* pFunc, uint8_t aOpcode)
{
	assert(aOpcode != 0);
	assert(pAddr != nullptr && pFunc != nullptr);

	if (aOpcode == 0)
		return;

	if (pAddr == nullptr && pFunc == nullptr)
		return;

	//LOG_INFO("WriteFunc: Splicing address '{}' with function '{}'.", pAddr, pFunc);

	WritePrimitive<uint8_t>(pAddr, aOpcode, true);
	Memoria::WriteRelative(Memoria::Advance(pAddr, 1), pFunc, true);
}

void Memoria::WriteCall(void* pAddr, void* pFunc)
{
	WriteFunc(pAddr, pFunc, 0xE8);
}

void Memoria::WriteJump(void* pAddr, void* pFunc)
{
	WriteFunc(pAddr, pFunc, 0xE9);
}

bool Memoria::CheckUInt8(void* pAddr, uint8_t nValue, int nOffset)
{
	return CheckPrimitive(pAddr, nValue, nOffset);
}

bool Memoria::CheckUInt16(void* pAddr, uint16_t nValue, int nOffset)
{
	return CheckPrimitive(pAddr, nValue, nOffset);
}

bool Memoria::CheckUInt32(void* pAddr, uint32_t nValue, int nOffset)
{
	return CheckPrimitive(pAddr, nValue, nOffset);
}

bool Memoria::CheckUInt64(void* pAddr, uint64_t nValue, int nOffset)
{
	return CheckPrimitive(pAddr, nValue, nOffset);
}

static Memoria::GetInstructionLength_t GetInstrLenCb{};

Memoria::GetInstructionLength_t Memoria::GetInstructionLengthCallback()
{
	return GetInstrLenCb;
}

void Memoria::SetInstructionLengthCallback(GetInstructionLength_t pfnCallback)
{
	GetInstrLenCb = pfnCallback;
}

void Memoria::EnumVirtualMemory(HANDLE processHandle, const std::function<bool(const MEMORY_BASIC_INFORMATION&, LPVOID)>& predicate, LPVOID lpParameter)
{
	MEMORY_BASIC_INFORMATION mbi;
	unsigned char* address = 0;

	while (VirtualQueryEx(processHandle, address, &mbi, sizeof(mbi)))
	{
		if (!predicate(mbi, lpParameter))
			break;

		address += mbi.RegionSize;
		if (address == NULL)
			break;
	}
}

void Memoria::EnumVirtualMemory(const std::function<bool(const MEMORY_BASIC_INFORMATION&, LPVOID)>& predicate, LPVOID lpParameter)
{
	return Memoria::EnumVirtualMemory(GetCurrentProcess(), predicate, lpParameter);
}

void* Memoria::HookRegular(void* pAddr, void* pFunc, size_t nCodeSize)
{
	assert(pAddr != nullptr && pFunc != nullptr);

	if (!pAddr || !pFunc)
		return nullptr;

	if (nCodeSize == 0)
	{
		while (nCodeSize < 5)
		{
			size_t size = GetInstrLenCb ? GetInstrLenCb(Memoria::Advance(pAddr, nCodeSize)) : 0;

			if (size == 0)
				return nullptr;

			nCodeSize += size;
		}
	}

	void* hook_dest_addr;

	if (Memoria::CheckUInt8(pAddr, 0xE9))
		hook_dest_addr = Memoria::Resolve(pAddr, 1, 4);
	else
		hook_dest_addr = Memoria::Advance(pAddr, nCodeSize);

	auto detour = std::make_unique<DetourInfo>(pAddr, hook_dest_addr, pFunc, nCodeSize);

	HookInfos.push_front(std::move(detour));

	return HookInfos.front()->GetCode();
}

void* Memoria::HookExport(HMODULE hModule, const std::string& sFuncName, void* pFuncAddr)
{
	if (!hModule || sFuncName.empty() || !pFuncAddr)
		return nullptr;

	void* p = GetProcAddress(hModule, sFuncName.c_str());
	if (p == nullptr)
		return nullptr;

	return HookRegular(p, pFuncAddr, 0);
}

void* Memoria::HookExport(const std::string& sModuleName, const std::string& sFuncName, void* pFuncAddr)
{
	if (sModuleName.empty())
		return nullptr;

	auto hModule = GetModuleHandleA(sModuleName.c_str());
	return Memoria::HookExport(hModule, sFuncName, pFuncAddr);
}

int Memoria::HookRefAddr(void* pHookAddr, void* pDetourAddr, void* pLowerBound, void* pUpperBound, uint8_t opcode)
{
	assert(pHookAddr != nullptr);
	assert(pDetourAddr != nullptr);
	assert(pLowerBound != nullptr);
	assert(pUpperBound != nullptr);

	if (!pHookAddr || !pDetourAddr || !pLowerBound || !pUpperBound)
		return 0;

	int opSize = opcode > 0 ? 1 : 0;
	void* p = pLowerBound;
	pUpperBound = Memoria::Advance(pUpperBound, -5);
	int result = 0;

	if (opcode != 0)
	{
		do
		{
			p = FindRelative(p, pLowerBound, pUpperBound, opcode);
			if (p == nullptr)
				return result;

			if (Memoria::Resolve(Memoria::Advance(p, 1), 4) == pHookAddr)
			{
				RefHookInfos.emplace_front(std::make_unique<RefDetourInfo>(p, pDetourAddr, opcode));
				++result;
			}

			p = Memoria::Advance(p, opSize + sizeof(pHookAddr));
		} while (true);
	}
	else
	{
		do
		{
			p = FindPrimitive(p, pLowerBound, pUpperBound, pHookAddr, false);
			if (p == nullptr)
				return result;

			RefHookInfos.emplace_front(std::make_unique<RefDetourInfo>(p, pDetourAddr, opcode));
			p = Memoria::Advance(p, sizeof(pHookAddr));

			++result;
		} while (true);
	}
	return result;
}

int Memoria::HookRefCall(void* pHookAddr, void* pDetourAddr, void* pLowerBound, void* pUpperBound)
{
	return HookRefAddr(pHookAddr, pDetourAddr, pLowerBound, pUpperBound, 0xE8);
}

int Memoria::HookRefJump(void* pHookAddr, void* pDetourAddr, void* pLowerBound, void* pUpperBound)
{
	return HookRefAddr(pHookAddr, pDetourAddr, pLowerBound, pUpperBound, 0xE9);
}

bool Memoria::RestoreHook(const void* pAddr)
{
	assert(!"not implemented");
	return false;

	//if (!pAddr)
	//	return false;
	//
	//auto detour = reinterpret_cast<DetourInfo *>(Memoria::Advance(const_cast<void *>(pAddr), -(signed)sizeof(DetourInfo)));
	//
	//auto pDetour = std::find_if(HookInfos.begin(), HookInfos.end(), [detour](const std::unique_ptr<DetourInfo> &ptr) -> bool
	//	{
	//		return (ptr.get() == detour);
	//	});
	//
	//if (pDetour == HookInfos.end())
	//{
	//	assert(false);
	//	return false;
	//}
	//
	//void *original = Memoria::Resolve(Memoria::Advance(detour->GetJump(), 1), 4);
	//Memoria::SetMemoryBlock(original, detour->Backup.data(), detour->Backup.size(), true);
	//
	//HookInfos.erase(pDetour);
	//
	//return true;
}

static bool SymInited = false;

static int SymShutdown()
{
	if (SymInited)
		SymCleanup(GetCurrentProcess());

	return 0;
}

static void SymInit()
{
	SymInited = SymInitialize(GetCurrentProcess(), NULL, TRUE);
	onexit(SymShutdown);
}

void* Memoria::GetFunctionBaseAddressFromItsCode(void* pAddress)
{
	HMODULE hModule = static_cast<HMODULE>(GetBaseAddress(pAddress));
	if (!hModule)
		return nullptr;

	ULONG size;
	PIMAGE_RUNTIME_FUNCTION_ENTRY pFunc = reinterpret_cast<PIMAGE_RUNTIME_FUNCTION_ENTRY>(
		ImageDirectoryEntryToData(hModule, TRUE, IMAGE_DIRECTORY_ENTRY_EXCEPTION, &size));

	if (!pFunc)
		return nullptr;

	DWORD count = size / sizeof(IMAGE_RUNTIME_FUNCTION_ENTRY);

	for (size_t i = 0u; i < count; ++i)
	{
		uintptr_t addr_needed = reinterpret_cast<uintptr_t>(pAddress);
		uintptr_t addr_begin = reinterpret_cast<uintptr_t>(hModule) + pFunc[i].BeginAddress;
		uintptr_t addr_end = reinterpret_cast<uintptr_t>(hModule) + pFunc[i].EndAddress;

		if (IsWithinBounds(addr_needed, addr_begin, addr_end))
			return reinterpret_cast<void*>(addr_begin);
	}

	return nullptr;
}

std::string Memoria::GetBeautyFunctionAddress(void* pAddress, bool concat_module_name, bool memory_beautify_on_error)
{
	auto pBaseAddress = Memoria::GetFunctionBaseAddressFromItsCode(pAddress);
	if (!pBaseAddress)
	{
		if (memory_beautify_on_error)
			return Memoria::BeautifyPointer(pAddress);

		return "";
	}

	auto sName = Memoria::GetSymbolName(pBaseAddress);
	if (sName.empty())
	{
		if (memory_beautify_on_error)
			return Memoria::BeautifyPointer(pAddress);

		return "";
	}

	auto offset = reinterpret_cast<uintptr_t>(pAddress) - reinterpret_cast<uintptr_t>(pBaseAddress);

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "+0x%llX", static_cast<unsigned long long>(offset));

	std::string result{};

	if (concat_module_name)
	{
		HMODULE hModule = reinterpret_cast<HMODULE>(Memoria::GetBaseAddress(pAddress));

		if (hModule != nullptr)
			result += Memoria::GetModuleName(hModule) + "!";
	}

	result += sName + buffer;
	result += " [" + Memoria::BeautifyPointer(pAddress) + "]";

	return result;
}

std::string Memoria::GetSymbolName(void* pAddress)
{
	static bool ensure_sym_init = (SymInit(), true);

	if (!SymInited)
		return "";

	DWORD64 displacement = 0;
	char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(buffer);
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = MAX_SYM_NAME;

	if (!SymFromAddr(GetCurrentProcess(), reinterpret_cast<DWORD64>(pAddress), &displacement, symbol))
		return "";

	return symbol->Name ? symbol->Name : "";
}

PIMAGE_SECTION_HEADER Memoria::GetSectionByIndex(HMODULE hModule, DWORD nImageDirectory, ULONG* nSize)
{
	if (!hModule)
		return nullptr;

	if (nImageDirectory >= IMAGE_NUMBEROF_DIRECTORY_ENTRIES)
		return nullptr;

	ULONG size = 0;
	void* directoryData = ImageDirectoryEntryToData(hModule, TRUE, nImageDirectory, &size);

	if (!directoryData || size == 0)
		return nullptr;

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)dosHeader + dosHeader->e_lfanew);

	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);

	for (unsigned int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
	{
		DWORD sectionStart = section->VirtualAddress;
		DWORD sectionEnd = sectionStart + section->SizeOfRawData;

		DWORD directoryVA = (DWORD)((DWORD_PTR)directoryData - (DWORD_PTR)hModule);

		if (directoryVA >= sectionStart && directoryVA < sectionEnd)
		{
			if (nSize)
				*nSize = size;

			return section;
		}

		section++;
	}

	return nullptr;
}

PIMAGE_SECTION_HEADER Memoria::GetSectionByFlags(HMODULE hModule, DWORD flags, bool isPedantic)
{
	if (!hModule)
		return nullptr;

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)dosHeader + dosHeader->e_lfanew);

	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);

	for (unsigned int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
	{
		if (isPedantic)
		{
			if (section->Characteristics == flags)
			{
				return section;
			}
		}
		else
		{
			if ((section->Characteristics & flags) != 0)
			{
				return section;
			}
		}

		section++;
	}

	return nullptr;
}

PIMAGE_SECTION_HEADER Memoria::GetSectionByName(HMODULE hModule, const std::string& name)
{
	if (!hModule || name.empty())
		return nullptr;

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)dosHeader + dosHeader->e_lfanew);

	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);

	for (unsigned int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
	{
		if (memcmp(section->Name, name.c_str(), name.size()) == 0)
		{
			return section;
		}

		section++;
	}
	return nullptr;
}

PIMAGE_SECTION_HEADER Memoria::GetRDataSection(HMODULE hModule)
{
	return GetSectionByFlags(hModule, IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ, false);
}

PIMAGE_SECTION_HEADER Memoria::GetMainCodeSection(HMODULE hModule)
{
	return GetSectionByFlags(hModule, IMAGE_SCN_CNT_CODE, true);
}

std::tuple<bool, PVOID, PVOID> Memoria::GetSectionBounds(HMODULE hModule, PIMAGE_SECTION_HEADER section)
{
	if (!hModule)
		return {};

	auto sectionStart = (PVOID)((DWORD_PTR)hModule + section->VirtualAddress);
	auto sectionEnd = (PVOID)((DWORD_PTR)sectionStart + section->Misc.VirtualSize - 1);

	return std::make_tuple(true, sectionStart, sectionEnd);
}

std::tuple<bool, PVOID, PVOID> Memoria::GetSectionBounds(PIMAGE_SECTION_HEADER section)
{
	HMODULE hModule;

	if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCSTR)section, &hModule))
		return {};

	if (!hModule)
		return {};

	return GetSectionBounds(hModule, section);
}

namespace RTTIDetails
{
	// Structure that represents the RTTI type descriptor
	struct RTTITypeDescriptor
	{
		// Always points to type_info's VFTable
		void** VFTable;
		// ?
		void* Spare;
		// Class Name
		char Name[1];
	};

	// Structure for Pointer-to-member displacement info
	struct PtrToMember
	{
		// VFTable offset (if PMD.PDisp is -1)
		int MDisp;
		// VBTable offset (-1: VFTable is at displacement PMD.MDisp is -1)
		int PDisp;
		// Displacement of the base class VFTable pointer inside the VBTable
		int VDisp;
	};

	// Structure for the base class descriptor
	struct RTTIBaseClassDescriptor
	{
		// TypeDescriptor of this base class
		RTTITypeDescriptor* TypeDescriptor;
		// Number of direct bases of this base class
		unsigned long NumContainedBases;
		// Pointer-to-member displacement info
		PtrToMember Where;
		// Flags, usually 0
		unsigned long Attributes;
	};

	// Structure for the base class array
	struct RTTIBaseClassArray
	{
		// Array of base class descriptors
		RTTIBaseClassDescriptor* ArrayOfBaseClassDescriptors[1];
	};

	// Structure for the class hierarchy descriptor
	struct RTTIClassHierarchyDescriptor
	{
		// Always 0?
		unsigned long Signature;
		// Bit 0 - multiple inheritance; Bit 1 - virtual inheritance
		unsigned long Attributes;
		// Number of base classes. Count includes the class itself
		unsigned long NumBaseClasses;
		// Array of base class descriptors
		RTTIBaseClassArray* BaseClassArray;
	};

	// Structure for the complete object locator
	struct RTTICompleteObjectLocator
	{
		// Always 0?
		unsigned long Signature;
		// Offset of VFTable within the class
		unsigned long Offset;
		// Constructor displacement offset
		unsigned long CDOffset;
		// Class Information
		RTTITypeDescriptor* TypeDescriptor;
		// Class Hierarchy information
		RTTIClassHierarchyDescriptor* ClassDescriptor;
	};
};

void* Memoria::GetRTTIDescriptor(void* pAddr, void* pLowerBound, void* pUpperBound, const std::string& sRttiName)
{
	using namespace RTTIDetails;

	constexpr unsigned long CLASS_SIGNATURE = 'VA?.';

	std::string nameEx = sRttiName;
	if (nameEx.empty())
		return nullptr;

	bool isFull = false;

	if (*(uint32_t*)nameEx.c_str() == CLASS_SIGNATURE)
	{
		isFull = true;
	}
	else
	{
		isFull = false;
		nameEx += '@';
	}

	void* p = pAddr;
	RTTITypeDescriptor* typeDescriptor;
	do
	{
		p = FindPrimitive(p, pLowerBound, pUpperBound, CLASS_SIGNATURE, false);
		if (!p)
			return nullptr;

		p = Memoria::Advance(p, 4);
		typeDescriptor = (RTTITypeDescriptor*)((char*)p - sizeof(void*) * 3);

		if (isFull)
		{
			if (_stricmp(typeDescriptor->Name, nameEx.c_str()) == 0)
				return typeDescriptor;
		}
		else
		{
			if (_strnicmp(&(typeDescriptor->Name[4]), nameEx.c_str(), nameEx.size()) == 0)
				return typeDescriptor;
		}
	} while (true);

	return nullptr;
}

void** Memoria::GetVTableForDescriptor(void* pAddr, void* pLowerBound, void* pUpperBound, void* pRttiTypeDescriptor)
{
	using namespace RTTIDetails;

	void* p = pAddr;

	do
	{
		p = Memoria::FindReference(p, pLowerBound, pUpperBound, pRttiTypeDescriptor);
		if (!p)
			return nullptr;

		RTTICompleteObjectLocator* l = (RTTICompleteObjectLocator*)((char*)p - sizeof(unsigned long) * 3);

		if (l->Signature == 0 && l->Offset == 0 && l->CDOffset == 0)
		{
			p = Memoria::FindReference(pAddr, pLowerBound, pUpperBound, l);

			if (p)
			{
				return (void**)Memoria::Advance(p, sizeof(void*));
			}

			return nullptr;
		}

		p = Memoria::Advance(p, 1);
	} while (true);

	return nullptr;
}

void** Memoria::GetVTableForClass(void* pAddr, void* pLowerBound, void* pUpperBound, const std::string& sRttiName)
{
	auto desc = GetRTTIDescriptor(pAddr, pLowerBound, pUpperBound, sRttiName);
	if (!desc)
		return nullptr;

	return GetVTableForDescriptor(pAddr, pLowerBound, pUpperBound, desc);
}

void* Memoria::GetImageBase(HMODULE hModule)
{
	if (!hModule)
		return nullptr;

	IMAGE_DOS_HEADER* pDOSHeader = (IMAGE_DOS_HEADER*)hModule;

	if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return nullptr;

	IMAGE_NT_HEADERS* pNTHeaders = (IMAGE_NT_HEADERS*)((BYTE*)pDOSHeader + pDOSHeader->e_lfanew);

	if (pNTHeaders->Signature != IMAGE_NT_SIGNATURE)
		return nullptr;

	return (void*)pNTHeaders->OptionalHeader.ImageBase;
}

void* Memoria::GetImageBase(const std::string& sModuleName)
{
	if (sModuleName.empty())
		return nullptr;

	auto hModule = GetModuleHandleA(sModuleName.c_str());
	if (!hModule)
		return nullptr;

	return Memoria::GetImageBase(hModule);
}

std::tuple<WORD, WORD> Memoria::GetOSVersionFromModule(HMODULE hModule)
{
	if (!hModule)
		return { 0, 0 };

	IMAGE_DOS_HEADER* pDOSHeader = (IMAGE_DOS_HEADER*)hModule;
	if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return { 0, 0 };

	IMAGE_NT_HEADERS* pNTHeaders = (IMAGE_NT_HEADERS*)((BYTE*)pDOSHeader + pDOSHeader->e_lfanew);
	if (pNTHeaders->Signature != IMAGE_NT_SIGNATURE)
		return { 0, 0 };

	return
	{
		pNTHeaders->OptionalHeader.MajorOperatingSystemVersion,
		pNTHeaders->OptionalHeader.MinorOperatingSystemVersion
	};
}

std::tuple<WORD, WORD> Memoria::GetOSVersionFromModuleName(const std::string& sModuleName)
{
	if (sModuleName.empty())
		return { 0, 0 };

	HMODULE hModule = GetModuleHandleA(sModuleName.c_str());
	if (!hModule)
		return { 0, 0 };

	return GetOSVersionFromModule(hModule);
}

std::string Memoria::GetModuleName(HMODULE hModule)
{
	char buffer[MAX_PATH];
	if (GetModuleFileNameA(hModule, buffer, MAX_PATH) == 0)
		return {};

	std::filesystem::path modulePath(buffer);

	return modulePath.filename().string();
}

std::string Memoria::GetModuleNameForAddress(void* pAddress)
{
	auto hBase = (HMODULE)Memoria::GetBaseAddress(pAddress);
	if (!hBase)
		return {};

	return Memoria::GetModuleName(hBase);
}

std::string Memoria::BeautifyPointer(void* addr)
{
	if (!addr)
		return "null";

	void* base = GetBaseAddress(addr);
	if (!base)
	{
		std::stringstream stream;
		stream << std::setfill('0') << std::setw(8) << std::hex << reinterpret_cast<intptr_t>(addr);
		return stream.str();
	}

	std::string name = Memoria::GetModuleName((HMODULE)base);

	const size_t last_slash_idx = name.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
		name.erase(0, last_slash_idx + 1);

	const size_t period_idx = name.rfind('.');
	if (std::string::npos != period_idx)
		name.erase(period_idx);

	std::stringstream result;
	result << name << "." << std::setfill('0') << std::setw(8) << std::hex
		<< reinterpret_cast<intptr_t>(addr) - reinterpret_cast<intptr_t>(base);

	return result.str();
}

std::vector<std::optional<uint8_t>> Memoria::CreateStdSignature(const std::string_view& str_sig)
{
	CSignature sig(str_sig);

	std::vector<std::optional<uint8_t>> std_sig;
	std_sig.reserve(sig.m_pattern.size());

	for (size_t i = 0; i < sig.m_pattern.size(); i++)
	{
		if (sig.m_mask[i] == 'x')
			std_sig.push_back(sig.m_pattern[i]);
		else
			std_sig.push_back(std::nullopt);
	}

	return std_sig;
}

std::vector<std::optional<uint8_t>> Memoria::CreateStdSignature(const uint8_t* pattern, size_t size)
{
	std::vector<std::optional<uint8_t>> std_sig;
	std_sig.reserve(size);

	for (size_t i = 0; i < size; i++)
	{
		if (pattern[i] == 0xFF)
			std_sig.push_back(std::nullopt);
		else
			std_sig.push_back(pattern[i]);
	}

	return std_sig;
}

Memoria::CSignature Memoria::CreateSignature(const std::string_view& str_sig)
{
	return CSignature(str_sig);
}

std::stack<std::pair<size_t, DWORD>>& Memoria::Dbg::GetProtectionStack()
{
	return ProtectionStack;
}

std::list<std::unique_ptr<DetourInfo>>& Memoria::Dbg::GetHooks()
{
	return HookInfos;
}

std::list<std::unique_ptr<RefDetourInfo>>& Memoria::Dbg::GetRefHooks()
{
	return RefHookInfos;
}

std::list<Memoria::CMemoryPatch>& Memoria::Dbg::GetPatches()
{
	return MemoryPatches;
}

RefDetourInfo::RefDetourInfo(void* original_address, void* hook_address, uint8_t opcode)
	: _original_function(original_address)
	, _hook_address(hook_address)
	, _opcode(opcode)
{
	if (opcode != 0)
	{
		Memoria::WriteFunc(original_address, hook_address, opcode);

		_patch_address = Memoria::GetPatchInfo(0);
		_patch_opcode = Memoria::GetPatchInfo(1);
	}
	else
	{
		WritePrimitive(original_address, hook_address, true);

		_patch_address = Memoria::GetPatchInfo(0);
		_patch_opcode = nullptr;
	}

	std::memcpy(_backup, _original_function, sizeof(_backup));
}

RefDetourInfo::~RefDetourInfo()
{
	if (_patch_opcode)
		_patch_opcode->Restore();

	if (_patch_address)
		_patch_address->Restore();
}

DetourInfo::DetourInfo(void* pAddress, void* pOriginal, void* pHook, size_t aCodeSize)
	: _address_orig(pAddress)
	, _address_hook(pHook)
	, _code_size(aCodeSize)
	, _code_addr(nullptr)
	, _patch_opcode(nullptr)
	, _patch_reladdr(nullptr)
{
	if (!pAddress || !pOriginal)
		return;

	const bool is_jmp = *(uint8_t*)pAddress == 0xE9;

	if (is_jmp)
	{
		_code_addr = Memoria::AllocMemory(5, true, true, true);
		Memoria::WriteJump(GetCode(), pOriginal);
	}
	else
	{
		_code_addr = Memoria::AllocMemory(aCodeSize + 5, true, true, true);
		std::memcpy(GetCode(), pAddress, aCodeSize);
		Memoria::WriteJump(GetJump(), pOriginal);
	}

	Memoria::WriteJump(pAddress, pHook);

	_patch_reladdr = Memoria::GetPatchInfo(0);
	_patch_opcode = Memoria::GetPatchInfo(1);
}

DetourInfo::~DetourInfo()
{

}

void* DetourInfo::GetCode() const
{
	return _code_addr;
}

void* DetourInfo::GetJump() const
{
	return reinterpret_cast<void*>(uintptr_t(_code_addr) + _code_size);
}

bool DetourInfo::IsActive() const
{
	assert(_patch_reladdr->IsActive() == _patch_opcode->IsActive());

	if (_patch_reladdr && _patch_opcode)
		return _patch_reladdr->IsActive() && _patch_opcode->IsActive();

	assert(false);
	return false;
}

void DetourInfo::Toggle(bool state)
{
	assert(_patch_reladdr->IsActive() == _patch_opcode->IsActive());

	if (_patch_reladdr)
		state ? _patch_reladdr->Apply() : _patch_reladdr->Restore();

	if (_patch_opcode)
		state ? _patch_opcode->Apply() : _patch_opcode->Restore();
}

bool RefDetourInfo::IsActive() const
{
	if (_patch_opcode && _patch_address)
	{
		return _patch_opcode->IsActive() && _patch_address->IsActive();
	}

	if (_patch_address)
	{
		return _patch_address->IsActive();
	}

	assert(false);
	return false;
}

void RefDetourInfo::Toggle(bool state)
{
	if (_patch_opcode)
		state ? _patch_opcode->Apply() : _patch_opcode->Restore();

	if (_patch_address)
		state ? _patch_address->Apply() : _patch_address->Restore();
}

std::vector<void*> Memoria::GetStackBacktrace()
{
	std::vector<void*> result{};

	result.clear();

	void* callers[128];
	int count = RtlCaptureStackBackTrace(0, _countof(callers), callers, NULL);

	for (int i = 2; i < count; i++)
		result.push_back(callers[i]);

	return result;
}

std::vector<std::string> Memoria::GetBeautyStackBacktrace()
{
	std::vector<std::string> result{};

	auto backtrace = GetStackBacktrace();
	if (backtrace.empty())
		return result;

	result.reserve(backtrace.size());

	for (auto&& addr : backtrace)
	{
		auto sName = GetBeautyFunctionAddress(addr);
		result.emplace_back(sName);
	}

	return result;
}

DWORD Memoria::BeginThread(const std::function<void(LPVOID)>& fnFunction, LPVOID lpParameter)
{
	DWORD nThreadId;

	struct ThreadData
	{
		std::function<void(LPVOID)> fnFunction;
		LPVOID lpParameter;
	};

	ThreadData* pData = new ThreadData{ fnFunction, lpParameter };

	auto ThreadFunction = [](LPVOID pParam) -> DWORD
		{
			ThreadData* pData = static_cast<ThreadData*>(pParam);
			pData->fnFunction(pData->lpParameter);
			delete pData;
			return 0;
		};

	HANDLE hThread = CreateThread(NULL, 0, ThreadFunction, pData, 0, &nThreadId);
	if (hThread == NULL)
	{
		delete pData;
		return 0;
	}

	CloseHandle(hThread);
	return nThreadId;
}

DWORD Memoria::BeginThread(const std::function<void()>& fnFunction)
{
	return Memoria::BeginThread([fnFunction](LPVOID) -> void { fnFunction(); }, nullptr);
}