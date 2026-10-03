#pragma once

struct SegmentInfo_t
{
	SegmentInfo_t() :
		Name{}, Base(nullptr), LastByte(nullptr), Size(0)
	{}

	std::string Name;
	void* Base;
	void* LastByte;
	uint32_t Size;
};

class CModule
{
private:
	CModule(const CModule&) = delete; // Prevents copy constructor
	CModule& operator=(const CModule&) = delete; // Prevents assignment

private:
	std::string m_sName, m_sNameNoExt;

	HMODULE m_hBase;
	size_t m_nSize;
	void* m_pEnd;

	SegmentInfo_t m_siSegmentCode;
	SegmentInfo_t m_siSegmentData;
	SegmentInfo_t m_siSegmentRData;

	std::list<std::shared_ptr<CSearchPattern>> m_Patterns;

private:
	void Initialization(void* pData, const std::string& aName, size_t nModuleSize);

public:
	CModule();
	CModule(const std::string& aName, size_t nModuleSize = 0);
	CModule(HMODULE aHandle, size_t nModuleSize = 0);
	CModule(void* aBase, size_t nModuleSize = 0);

	bool GetLoaded() const;
	std::string GetName(bool noExtension = true) const;
	void* GetBase() const;
	size_t GetSize() const;
	void* GetLastByte() const;

	const SegmentInfo_t& GetSegmentCode() const;
	const SegmentInfo_t& GetSegmentData() const;
	const SegmentInfo_t& GetSegmentRData() const;

	std::shared_ptr<CSearchPattern> CreatePatternNoVar(const std::string& sName);
	std::shared_ptr<CSearchPattern> CreatePattern();

	template<typename T>
	std::shared_ptr<CSearchPattern> CreatePattern(T& param, const std::string& sName = {})
	{
		m_Patterns.emplace_back(std::make_shared<CSearchPattern>(this, reinterpret_cast<void**>(&param), sName));
		return m_Patterns.back();
	};

	const auto& GetPatterns() const { return m_Patterns; };

	int HookRefAddr(void* pAddr, void* pNewAddr, uint8_t nOpcode = 0);
	int HookRefCall(void* pAddr, void* pNewAddr);
	int HookRefJump(void* pAddr, void* pNewAddr);

	void* GetExport(const std::string& sFuncName);
	void* HookExport(const std::string& sFuncName, void* pNewAddr);

	void* Transpose(ptrdiff_t nOffset);

	bool InBounds(void* addr);

	void* FindString(const char* pszString, ptrdiff_t nOffset, bool bZeroed);
	void* FindStringInCode(const char* pszPattern, ptrdiff_t nOffset = 0, bool bZeroed = false);

	void* FindNearCallOpcodeEx(void* pStart, bool back = false);
};

namespace Memoria
{
	std::unique_ptr<CModule> CreateModule(const std::string& aName, size_t nModuleSize = 0);
	std::unique_ptr<CModule> CreateModule(HMODULE aHandle, size_t nModuleSize = 0);
	std::unique_ptr<CModule> CreateModule(void* pData, size_t nModuleSize = 0);
	std::unique_ptr<CModule> CreateModule(std::nullptr_t, size_t nModuleSize = 0);

	bool IsModuleReady(const std::string& aName);
}