#pragma once

class DetourInfo
{
private:
	void* _address_orig;
	void* _address_hook;

	size_t _code_size;
	void* _code_addr;

	Memoria::CMemoryPatch* _patch_opcode;
	Memoria::CMemoryPatch* _patch_reladdr;

public:
	DetourInfo(void* pAddress, void* pOriginal, void* pHook, size_t aCodeSize);
	~DetourInfo();

	void* GetOriginal() const { return _address_orig; }
	void* GetHook() const { return _address_hook; }

	void* GetCode() const;
	void* GetJump() const;

	bool IsActive() const;
	void Toggle(bool state);
};

struct RefDetourInfo
{
public:
	void* _original_function;
	uint8_t _backup[5];

	void* _hook_address;
	uint8_t _opcode;

	Memoria::CMemoryPatch* _patch_opcode;
	Memoria::CMemoryPatch* _patch_address;

public:
	RefDetourInfo(void* original_address, void* hook_address, uint8_t opcode);
	~RefDetourInfo();

	void* GetOriginal() const { return _original_function; }
	void* GetHook() const { return _hook_address; }

	bool IsActive() const;
	void Toggle(bool state);
};

namespace Memoria
{
	// Выделение памяти. Вся выделенная память контролируется Memoria и автоматически освобождается
	// при вызове `Cleanup` функции.
	//
	// Важно понимать, что тип выделяемой памяти - virtual, что означает, что размер памяти не будет полностью
	// соответствовать размеру `nSize`. Память будет выделена страницей, то есть 4096 байт, если размер 
	// от 1 до 4096, или 8192, если размер от 4097 до 8192 и т.д.
	extern void* AllocMemory(size_t nSize, bool bExecutable = false, bool bReadable = true, bool bWritable = true);

	// Освобождение памяти, выделенной с помощью Memoria.
	extern bool FreeMemory(void* pMemory);

	extern void* Resolve(void* pAddr, ptrdiff_t nPreOffset, ptrdiff_t nPostOffset);
	extern void* Resolve(void* pAddr, ptrdiff_t nOffset = 0);
	extern int32_t Relative(void* pBase, void* pAddr, size_t nInstrSize = 4);
	extern void* Advance(void* pAddr, ptrdiff_t nOffset, bool bDereference = false);

	extern bool IsWithinBounds(uintptr_t pAddress, uintptr_t pLowerBound, uintptr_t pUpperBound);
	extern bool IsWithinBounds(void* pAddress, void* pLowerBound, void* pUpperBound);

	extern bool IsMemoryValid(void* pAddress);
	extern bool IsMemoryExecutable(void* pAddress);

	extern bool PushMemoryProtection(void* pAddress, size_t nSize, DWORD nNewProtection);
	extern bool PushMemoryProtection(void* pAddress, size_t nSize); // TODO: Remove

	extern bool PopMemoryProtection(void* pAddress);
	extern bool PopMemoryProtection();

	extern CMemoryPatch* GetPatchInfo(size_t nIndex);

	extern void* GetBaseAddress(void* pAddress);
	extern void* GetRelativeAddress(void* pAddress);
	extern DWORD GetModuleSize(HMODULE hInstance);

	template <typename T> constexpr uint64_t FNV1a(const T* text);

	extern HMODULE GetModuleHandleDirect(uint64_t module_name_hash);
	extern void* GetProcAddressDirect(uint64_t module_name_hash, uint64_t function_name_hash);

	extern void* GetInterfaceAddress(HMODULE hModule, const std::string& sInterfaceName);
	extern void* GetInterfaceAddress(const std::string& sModuleName, const std::string& sInterfaceName);

	extern bool GetMemoryBlock(void* pSource, size_t nCount, void* pDest);
	extern std::span<uint8_t> GetMemorySpan(void* pSource, size_t nCount);
	extern std::vector<uint8_t> GetMemoryData(void* pSource, size_t nCount);

	extern bool SetMemoryBlock(void* pDest, const void* pSource, size_t nCount, bool bModifyProtect = true);
	extern bool SetMemorySpan(void* pDest, const std::span<const uint8_t>& data, bool bModifyProtect = true);
	extern bool SetMemoryData(void* pDest, const std::vector<uint8_t>& data, bool bModifyProtect = true);

	extern DWORD SetProtection(void* pCode, DWORD value);
	extern bool RestoreLastProtection();

	extern uint8_t* FindUInt8(void* pStart, int nLimit, uint8_t aValue, bool bBackward = false, int nOffset = 0);
	extern uint16_t* FindUInt16(void* pStart, int nLimit, uint16_t aValue, bool bBackward = false, int nOffset = 0);
	extern uint32_t* FindUInt32(void* pStart, int nLimit, uint32_t aValue, bool bBackward = false, int nOffset = 0);
	extern uint64_t* FindUInt64(void* pStart, int nLimit, uint64_t aValue, bool bBackward = false, int nOffset = 0);

	extern uint8_t* FindUInt8(void* pStart, void* pLowerBound, void* pUpperBound, uint8_t aValue, bool bBackward = false, int nOffset = 0);
	extern uint16_t* FindUInt16(void* pStart, void* pLowerBound, void* pUpperBound, uint16_t aValue, bool bBackward = false, int nOffset = 0);
	extern uint32_t* FindUInt32(void* pStart, void* pLowerBound, void* pUpperBound, uint32_t aValue, bool bBackward = false, int nOffset = 0);
	extern uint64_t* FindUInt64(void* pStart, void* pLowerBound, void* pUpperBound, uint64_t aValue, bool bBackward = false, int nOffset = 0);

	extern void* FindSignature(void* pStart, void* pLowerBound, void* pUpperBound, const std::string& signature, bool bBackward = false, int nOffset = 0);

	extern void* FindBlock(void* pStart, void* pLowerBound, void* pUpperBound, const void* pData, size_t nSize, bool bBackward = false, int nOffset = 0);
	extern void* FindBlock(void* pStart, void* pLowerBound, void* pUpperBound, const std::vector<std::optional<uint8_t>>& vValue, bool bBackward = false, int nOffset = 0);

	extern void* FindRelative(void* pStart, void* pLowerBound, void* pUpperBound, uint16_t nOpcode, int nIndex = 0, bool bBack = false, int nOffset = 0);
	extern void* FindReference(void* pStart, void* pLowerBound, void* pUpperBound, void* aRefAddr, uint16_t nOpcode = 0, bool bBack = false, int nOffset = 0);
	extern void* FindAnsiString(void* pStart, void* pLowerBound, void* pUpperBound, const char* aData, int nOffset = 0);

	extern void WriteRelative(void* pAddr, void* pValue, bool bModifyProtect = true);

	extern void WriteUInt8(void* pAddr, uint8_t nValue, bool bModifyProtect = true);
	extern void WriteUInt16(void* pAddr, uint16_t nValue, bool bModifyProtect = true);
	extern void WriteUInt32(void* pAddr, uint32_t nValue, bool bModifyProtect = true);
	extern void WriteUInt64(void* pAddr, uint64_t nValue, bool bModifyProtect = true);
	extern void WritePointer(void* pAddr, const void* pValue, bool bModifyProtect = true);

	extern void FillChar(void* pAddr, unsigned char cValue, size_t nSize, bool bModifyProtect = true);
	extern void FillNops(void* pAddr, size_t nSize, bool bModifyProtect = true);

	enum class eFuncOpcode
	{
		Jump, Call,
	};

	// TODO: eFuncOpcode
	extern void WriteFunc(void* pAddr, void* pFunc, uint8_t nOpcode);
	extern void WriteCall(void* pAddr, void* pFunc);
	extern void WriteJump(void* pAddr, void* pFunc);

	extern bool CheckUInt8(void* pAddr, uint8_t nValue, int nOffset = 0);
	extern bool CheckUInt16(void* pAddr, uint16_t nValue, int nOffset = 0);
	extern bool CheckUInt32(void* pAddr, uint32_t nValue, int nOffset = 0);
	extern bool CheckUInt64(void* pAddr, uint64_t nValue, int nOffset = 0);

	extern void* HookRegular(void* pHookAddr, void* pFunc, size_t nCodeSize = 0);

	extern void* HookExport(HMODULE hModule, const std::string& sFuncName, void* pFuncAddr);
	extern void* HookExport(const std::string& sModuleName, const std::string& sFuncName, void* pFuncAddr);

	extern int HookRefAddr(void* pHookAddr, void* pDetourAddr, void* pLowerBound, void* pUpperBound, uint8_t opcode);
	extern int HookRefCall(void* pHookAddr, void* pDetourAddr, void* pLowerBound, void* pUpperBound);
	extern int HookRefJump(void* pHookAddr, void* pDetourAddr, void* pLowerBound, void* pUpperBound);

	extern bool RestoreHook(const void* pHookAddr);

	// i.e. IMAGE_DIRECTORY_ENTRY_EXPORT
	extern PIMAGE_SECTION_HEADER GetSectionByIndex(HMODULE hModule, DWORD nImageDirectory, ULONG* nSize = nullptr);
	extern PIMAGE_SECTION_HEADER GetSectionByFlags(HMODULE hModule, DWORD flags, bool isPedantic = true);
	extern PIMAGE_SECTION_HEADER GetSectionByName(HMODULE hModule, const std::string& name);

	extern PIMAGE_SECTION_HEADER GetRDataSection(HMODULE hModule);
	extern PIMAGE_SECTION_HEADER GetMainCodeSection(HMODULE hModule);

	extern std::tuple<bool, PVOID, PVOID> GetSectionBounds(HMODULE hModule, PIMAGE_SECTION_HEADER section);
	extern std::tuple<bool, PVOID, PVOID> GetSectionBounds(PIMAGE_SECTION_HEADER section);

	extern void* GetRTTIDescriptor(void* pAddr, void* pLowerBound, void* pUpperBound, const std::string& sRttiName);
	extern void** GetVTableForDescriptor(void* pAddr, void* pLowerBound, void* pUpperBound, void* pRttiTypeDescriptor);
	extern void** GetVTableForClass(void* pAddr, void* pLowerBound, void* pUpperBound, const std::string& sRttiName);

	extern void* GetImageBase(HMODULE hModule);
	extern void* GetImageBase(const std::string& sModuleName);

	extern std::tuple<WORD, WORD> GetOSVersionFromModule(HMODULE hModule);
	extern std::tuple<WORD, WORD> GetOSVersionFromModuleName(const std::string& sModuleName);

	extern std::string GetModuleName(HMODULE hModule);
	extern std::string GetModuleNameForAddress(void* pAddress);
	extern std::string BeautifyPointer(void* pAddress);

	extern std::vector<std::optional<uint8_t>> CreateStdSignature(const std::string_view& str_sig);
	extern std::vector<std::optional<uint8_t>> CreateStdSignature(const uint8_t* pattern, size_t size);
	extern CSignature CreateSignature(const std::string_view& str_sig);

	using GetInstructionLength_t = size_t(*)(void*);

	extern GetInstructionLength_t GetInstructionLengthCallback();
	extern void SetInstructionLengthCallback(GetInstructionLength_t pfnCallback);

	extern void EnumVirtualMemory(HANDLE processHandle, const std::function<bool(const MEMORY_BASIC_INFORMATION&, LPVOID)>& predicate, LPVOID lpParameter = NULL);
	extern void EnumVirtualMemory(const std::function<bool(const MEMORY_BASIC_INFORMATION&, LPVOID)>& predicate, LPVOID lpParameter = NULL);

	extern std::string GetSymbolName(void* pAddress);
	extern std::string GetBeautyFunctionAddress(void* pAddress, bool concat_module_name = true, bool memory_beautify_on_error = true);
	extern void* GetFunctionBaseAddressFromItsCode(void* pAddress);

	extern std::vector<void*> GetStackBacktrace();
	extern std::vector<std::string> GetBeautyStackBacktrace();

	extern DWORD BeginThread(const std::function<void(LPVOID)>& fnFunction, LPVOID lpParameter = nullptr);
	extern DWORD BeginThread(const std::function<void()>& fnFunction);

	namespace Dbg
	{
		extern std::stack<std::pair<size_t, DWORD>>& GetProtectionStack();
		extern std::list<std::unique_ptr<DetourInfo>>& GetHooks();
		extern std::list<std::unique_ptr<RefDetourInfo>>& GetRefHooks();
		extern std::list<Memoria::CMemoryPatch>& GetPatches();
	}
}

template <typename T>
constexpr uint64_t Memoria::FNV1a(const T* text)
{
	const uint64_t prime = 0x00000100000001B3;
	uint64_t hash = 0xcbf29ce484222325;

	do
	{
		hash ^= static_cast<uint64_t>(*text);
		hash *= prime;
	} while (*text++);

	return hash;
}