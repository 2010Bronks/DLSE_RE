#include "precompiled.hpp"

static DWORD GetModuleSize(HMODULE hInstance)
{
	PIMAGE_DOS_HEADER dos = reinterpret_cast<PIMAGE_DOS_HEADER>(hInstance);
	PIMAGE_NT_HEADERS nt = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(dos) + dos->e_lfanew);

	return nt->OptionalHeader.SizeOfImage;
}
CModule::CModule()
{
	Initialization(nullptr, "", 0);
}

CModule::CModule(const std::string& aName, size_t nModuleSize)
{
	Initialization(nullptr, aName, nModuleSize);
}

CModule::CModule(HMODULE aHandle, size_t nModuleSize)
{
	Initialization((void*)aHandle, Memoria::GetModuleName(aHandle), nModuleSize);
}

CModule::CModule(void* aBase, size_t nModuleSize)
{
	Initialization(aBase, "", nModuleSize);
}

void CModule::Initialization(void* pData, const std::string& aName, size_t nModuleSize)
{
	PIMAGE_SECTION_HEADER Code{}, Data{}, RData{};

	m_sName = aName;

	if (pData != nullptr)
	{
		m_hBase = (HMODULE)pData;
	}
	else
	{
		if (aName.empty())
			m_hBase = GetModuleHandleA(NULL);
		else
			m_hBase = GetModuleHandleA(aName.c_str());
	}

	if (m_sName.empty())
		m_sName = std::format("0x{:X}", (uintptr_t)m_hBase);

	if (auto lastDot = m_sName.find_last_of("."); lastDot != std::string::npos)
	{
		m_sNameNoExt = m_sName.substr(0, lastDot);
	}
	else
	{
		m_sNameNoExt = m_sName;
	}

	if (m_hBase != 0)
	{
		if (nModuleSize == 0)
			m_nSize = GetModuleSize(m_hBase);
		else
			m_nSize = nModuleSize;

		m_pEnd = Transpose(m_nSize - 1);

		Code = Memoria::GetSectionByFlags(m_hBase, IMAGE_SCN_CNT_CODE, false);
		Data = Memoria::GetSectionByFlags(m_hBase, IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
		RData = Memoria::GetSectionByFlags(m_hBase, IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ);

		if (Code != nullptr)
		{
			m_siSegmentCode.Name = std::string(reinterpret_cast<char*>(Code->Name));
			auto&& [success, base, last_byte] = Memoria::GetSectionBounds(m_hBase, Code);

			if (success)
			{
				m_siSegmentCode.Base = base;
				m_siSegmentCode.LastByte = last_byte;
				m_siSegmentCode.Size = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(m_siSegmentCode.LastByte) - reinterpret_cast<uintptr_t>(m_siSegmentCode.Base) - 1);
			}
			else
			{
				m_siSegmentCode.Base = {};
				m_siSegmentCode.LastByte = {};
				m_siSegmentCode.Size = {};
			}
		}

		if (Data != nullptr)
		{
			m_siSegmentData.Name = std::string(reinterpret_cast<char*>(Data->Name));

			auto&& [success, base, last_byte] = Memoria::GetSectionBounds(m_hBase, Data);

			if (success)
			{
				m_siSegmentData.Base = base;
				m_siSegmentData.LastByte = last_byte;
				m_siSegmentData.Size = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(m_siSegmentData.LastByte) - reinterpret_cast<uintptr_t>(m_siSegmentData.Base) - 1);
			}
			else
			{
				m_siSegmentData.Base = {};
				m_siSegmentData.LastByte = {};
				m_siSegmentData.Size = {};
			}
		}

		if (RData != nullptr)
		{
			m_siSegmentRData.Name = std::string(reinterpret_cast<char*>(RData->Name));

			auto&& [success, base, last_byte] = Memoria::GetSectionBounds(m_hBase, RData);

			if (success)
			{
				m_siSegmentRData.Base = base;
				m_siSegmentRData.LastByte = last_byte;
				m_siSegmentRData.Size = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(m_siSegmentRData.LastByte) - reinterpret_cast<uintptr_t>(m_siSegmentRData.Base) - 1);
			}
			else
			{
				m_siSegmentRData.Base = {};
				m_siSegmentRData.LastByte = {};
				m_siSegmentRData.Size = {};
			}
		}
	}
	else
	{
		m_nSize = 0;
		m_pEnd = nullptr;
	}
}

int CModule::HookRefAddr(void* aAddr, void* aNewAddr, uint8_t aOpcode)
{
	return Memoria::HookRefAddr(aAddr, aNewAddr, m_hBase, m_pEnd, aOpcode);
}

int CModule::HookRefCall(void* aAddr, void* aNewAddr)
{
	return Memoria::HookRefAddr(aAddr, aNewAddr, m_siSegmentCode.Base, m_siSegmentCode.LastByte, 0xE8);
}

int CModule::HookRefJump(void* aAddr, void* aNewAddr)
{
	return Memoria::HookRefAddr(aAddr, aNewAddr, m_siSegmentCode.Base, m_siSegmentCode.LastByte, 0xE9);
}

void* CModule::GetExport(const std::string& sFuncName)
{
	if (sFuncName.empty())
		return nullptr;

	return GetProcAddress(m_hBase, sFuncName.c_str());
}

void* CModule::HookExport(const std::string& aFuncName, void* aNewAddr)
{
	if (aFuncName.empty() || aNewAddr == nullptr)
		return nullptr;

	return Memoria::HookExport(m_hBase, aFuncName.c_str(), aNewAddr);
}

bool CModule::GetLoaded() const
{
	return (m_hBase != NULL);
}

std::string CModule::GetName(bool noExtension) const
{
	return (noExtension) ? (m_sNameNoExt) : (m_sName);
}

void* CModule::GetBase() const
{
	return m_hBase;
}

size_t CModule::GetSize() const
{
	return m_nSize;
}

void* CModule::GetLastByte() const
{
	return m_pEnd;
}

const SegmentInfo_t& CModule::GetSegmentCode() const
{
	return m_siSegmentCode;
}

const SegmentInfo_t& CModule::GetSegmentData() const
{
	return m_siSegmentData;
}

const SegmentInfo_t& CModule::GetSegmentRData() const
{
	return m_siSegmentRData;
}

std::shared_ptr<CSearchPattern> CModule::CreatePatternNoVar(const std::string& sName)
{
	m_Patterns.emplace_back(std::make_shared<CSearchPattern>(this, nullptr, sName));
	return m_Patterns.back();
}

std::shared_ptr<CSearchPattern> CModule::CreatePattern()
{
	m_Patterns.emplace_back(std::make_shared<CSearchPattern>(this, nullptr, ""));
	return m_Patterns.back();
}

void* CModule::Transpose(ptrdiff_t aOffset)
{
	if (m_hBase == 0)
		return nullptr;

	return Memoria::Advance(m_hBase, aOffset);
}

bool CModule::InBounds(void* addr)
{
	if (m_hBase == 0)
		return false;

	return Memoria::IsWithinBounds(addr, m_hBase, m_pEnd);
}

void* CModule::FindString(const char* pszString, ptrdiff_t nOffset, bool bZeroed)
{
	size_t nSize = strlen(pszString) - ((bZeroed) ? 0 : 1);

	return Memoria::FindBlock(m_hBase, m_hBase, m_pEnd, pszString, nSize, false, nOffset);
}

void* CModule::FindStringInCode(const char* pszPattern, ptrdiff_t nOffset, bool bZeroed)
{
#ifdef _WIN64
	assert(!"Not implemented");
	return nullptr;
#else
	if (pszPattern == nullptr || *pszPattern == '\0')
		return nullptr;

	auto addr = FindString(pszPattern, 0, bZeroed);
	if (addr == nullptr)
		return nullptr;

	uint8_t push_xxx_pattern[5] = { 0x68, 0x00, 0x00, 0x00, 0x00 };

	*(void**)&push_xxx_pattern[1] = addr;

	return Memoria::FindBlock(m_siSegmentCode.Base, m_siSegmentCode.Base, m_siSegmentCode.LastByte,
		push_xxx_pattern, sizeof(push_xxx_pattern), false, nOffset);
#endif
}

void* CModule::FindNearCallOpcodeEx(void* pStart, bool back)
{
	auto addr = Memoria::FindRelative(pStart, m_hBase, m_pEnd, 0xE8, 0, back, 0);

	if (addr == nullptr)
		return nullptr;

	return Memoria::Resolve(addr, 1, 4);
}

std::unique_ptr<CModule> Memoria::CreateModule(const std::string& aName, size_t nModuleSize)
{
	return std::make_unique<CModule>(aName, nModuleSize);
}

std::unique_ptr<CModule> Memoria::CreateModule(HMODULE aHandle, size_t nModuleSize)
{
	return std::make_unique<CModule>(aHandle, nModuleSize);
}

std::unique_ptr<CModule> Memoria::CreateModule(void* pData, size_t nModuleSize)
{
	return std::make_unique<CModule>(pData, nModuleSize);
}

std::unique_ptr<CModule> Memoria::CreateModule(std::nullptr_t, size_t nModuleSize)
{
	return std::make_unique<CModule>("", nModuleSize);
}

bool Memoria::IsModuleReady(const std::string& aName)
{
	if (aName.empty())
		return GetModuleHandleA(NULL) != NULL;

	return GetModuleHandleA(aName.c_str()) != NULL;
}