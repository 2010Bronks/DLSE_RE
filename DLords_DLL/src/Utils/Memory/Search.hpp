#pragma once

class CModule;

// TODO: Getters (GetUInt8, GetUInt16, etc...)
class CSearchPattern
{
private:
	std::string m_sName;
	CModule* m_pModule;
	void** m_pOutput;

#ifdef _DEBUG
	void* m_pDebugOutput;
#endif

	bool m_bHasNoOutput;
	void* m_pInternalData;

private:
	void SetOutputInternally(void* aValue, bool bDerefValue);

public:
	std::string GetName() const { return m_sName; }
	const CModule* GetOwner() const { return m_pModule; }

	template <typename T = uintptr_t>
	FORCEINLINE T GetResult() const { return (T)(m_pOutput ? *m_pOutput : nullptr); }

	void* GetResultPtr() const { return (m_pOutput ? *m_pOutput : nullptr); }

	ptrdiff_t GetOffset() const;

	void FindUInt8(uint8_t aValue, bool bBackward = false);
	void FindUInt16(uint16_t aValue, bool bBackward = false);
	void FindUInt32(uint32_t aValue, bool bBackward = false);

	void FindSignature(const std::vector<std::optional<uint8_t>>& aValue, bool aBack = false);
	void FindSignature(const std::string& aValue, bool aBack = false);
	void FindSignature(const uint8_t* value, size_t size, bool back = false);

	/**
	* @brief Searches for an ANSI string throughout the module.
	*
	* @param aValue The string to search for.
	* @param bIsStringDeep If true, the search will be performed in all sections of the module, if false, then only in `.rdata`.
	* @param bIStringRef If true, after finding the `aValue` signature, the function will find the `push offset XXX` asm command that uses this string.
	* @param bIsStringPart If true, then `aValue` is guaranteed to be complete, if false, then the string is partial.
	*/
	void FindAnsiString(const char* aValue, bool bIsStringDeep = true, bool bIsStringRef = false, bool bIsStringPart = false);
	void FindWideString(const wchar_t* aValue, bool bIsStringDeep = true, bool bIsStringRef = false, bool bIsStringPart = false);

	void FindRelative(uint16_t aOpcode = 0, int aIndex = 0, bool aBack = false);
	void FindReference();
	void FindSelf();

	void FindCall(int aIndex = 0, bool bJumpIn = false, bool aBack = false);
	void FindNextCall(bool bJumpIn = false);
	void FindPrevCall(bool bJumpIn = false);

	void JoinCall(int aIndex = 0, bool aBack = false);
	void JoinNextCall();
	void JoinPrevCall();

	void FindJump(int aIndex = 0, bool bJumpIn = false, bool aBack = false);
	void FindNextJump(bool bJumpIn = false);
	void FindPrevJump(bool bJumpIn = false);

	void JoinJump(int aIndex = 0, bool aBack = false);
	void JoinNextJump();
	void JoinPrevJump();

	void FindVTable(const std::string& aName);

	bool CheckUInt8(uint8_t aValue, int aOffset);
	bool CheckUInt16(uint16_t aValue, int aOffset);
	bool CheckUInt32(uint32_t aValue, int aOffset);

	void GetExport(const std::string& name);
	void GetInterface(const std::string& name);

	void Dereference();

	void Resolve();
	void Resolve(int aOffset);
	void Resolve(int aPreOffset, int aPostOffset);

	void Advance(ptrdiff_t aValue);
	void Add(size_t aValue);
	void Sub(size_t aValue);

	void Align(size_t nValue = 0x10);

	void Reset();
	void Invalidate();
	void ForceOutput(void* aValue);
	void* CurrentOutput();

	bool IsValid();

	CSearchPattern(CModule* aModule, void** aOutput = nullptr, const std::string& aName = "");
	~CSearchPattern();
};