#include "precompiled.hpp"

static bool IsWithinBounds(uintptr_t pAddress, uintptr_t pLowerBound, uintptr_t pUpperBound)
{
	if (pAddress < pLowerBound)
		return false;

	if (pAddress >= pUpperBound)
		return false;

	return true;
}

static bool IsWithinBounds(void* pAddress, void* pLowerBound, void* pUpperBound)
{
	auto addr = reinterpret_cast<uintptr_t>(pAddress);
	auto lower = reinterpret_cast<uintptr_t>(pLowerBound);
	auto upper = reinterpret_cast<uintptr_t>(pUpperBound);

	return IsWithinBounds(addr, lower, upper);
}

template <typename T>
static T* FindPrimitive(void* pStart, void* pLowerBound, void* pUpperBound, T aValue, bool back, int nOffset = 0)
{
	assert(pLowerBound != nullptr && pUpperBound != nullptr && pLowerBound <= pUpperBound);

	pUpperBound = reinterpret_cast<T*>(reinterpret_cast<intptr_t>(pUpperBound) - sizeof(T));

	T* result = (T*)pStart;

	do
	{
		if (!IsWithinBounds(result, pLowerBound, pUpperBound))
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
		result = Advance(result, nOffset);

	return static_cast<T*>(result);
}

ptrdiff_t CSearchPattern::GetOffset() const
{
	if (m_pOutput == nullptr || m_pModule == nullptr)
		return 0;

	if (*m_pOutput == nullptr)
		return 0;

	return (ptrdiff_t)(uintptr_t(*m_pOutput) - (uintptr_t)m_pModule->GetBase());
}

void CSearchPattern::SetOutputInternally(void* aValue, bool bDerefValue)
{
	if (!m_pOutput)
		return;

	__try
	{
		if (bDerefValue)
			*m_pOutput = *(void**)aValue;
		else
			*m_pOutput = aValue;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		assert(false);
		*m_pOutput = {};
	}

#ifdef _DEBUG
	m_pDebugOutput = aValue;
#endif
}

void CSearchPattern::FindUInt8(uint8_t aValue, bool bBack)
{
	if (*m_pOutput == nullptr)
		return;

	auto result = Memoria::FindUInt8(*m_pOutput, m_pModule->GetBase(), m_pModule->GetLastByte(), aValue, bBack);

	SetOutputInternally(result, false);
}

void CSearchPattern::FindUInt16(uint16_t aValue, bool bBack)
{
	if (*m_pOutput == nullptr)
		return;

	auto result = Memoria::FindUInt16(*m_pOutput, m_pModule->GetBase(), m_pModule->GetLastByte(), aValue, bBack);

	SetOutputInternally(result, false);
}

void CSearchPattern::FindUInt32(uint32_t aValue, bool bBack)
{
	if (*m_pOutput == nullptr)
		return;

	auto result = Memoria::FindUInt32(*m_pOutput, m_pModule->GetBase(), m_pModule->GetLastByte(), aValue, bBack);

	SetOutputInternally(result, false);
}

void CSearchPattern::FindSignature(const std::vector<std::optional<uint8_t>>& aValue, bool aBack)
{
	//LOG("[%bFindSignature%d] Name: {}, Output: {}", GetName(), CurrentOutput());

	if (*m_pOutput == nullptr)
		return;

	auto result = Memoria::FindBlock(*m_pOutput, m_pModule->GetBase(), m_pModule->GetLastByte(), aValue, aBack);

	SetOutputInternally(result, false);
}

void CSearchPattern::FindSignature(const std::string& aValue, bool aBack)
{
	//LOG("[%bFindSignature%d] Name: {}, Output: {}", GetName(), CurrentOutput());

	if (*m_pOutput == nullptr)
		return;

	auto sig = Memoria::CreateSignature(aValue);
	void* result;

	if (sig.m_hasOptionals)
		result = Memoria::FindBlock(*m_pOutput, m_pModule->GetBase(), m_pModule->GetLastByte(), sig.BuildStdSignature(), aBack);
	else
		result = Memoria::FindBlock(*m_pOutput, m_pModule->GetBase(), m_pModule->GetLastByte(), sig.m_pattern.data(), sig.m_pattern.size(), aBack);

	SetOutputInternally(result, false);
}

void CSearchPattern::FindSignature(const uint8_t* value, size_t size, bool back)
{
	if (*m_pOutput == nullptr || value == nullptr || size == 0)
	{
		SetOutputInternally({}, false);
		return;
	}

	auto sig = Memoria::CreateStdSignature(value, size);

	FindSignature(sig, back);
}

void CSearchPattern::FindAnsiString(const char* aValue, bool bIsStringDeep, bool bIsStringRef, bool bIsStringPart)
{
	if (*m_pOutput == nullptr || aValue == nullptr || *aValue == '\0')
	{
		SetOutputInternally({}, false);
		return;
	}

	void* result;
	size_t len = strlen(aValue) * sizeof(*aValue);

	if (!bIsStringPart)
		len += sizeof(*aValue);

	std::vector<std::optional<uint8_t>> pattern(len);
	std::copy(aValue, aValue + len, pattern.begin());

	if (bIsStringDeep)
	{
		result = Memoria::FindBlock(m_pModule->GetBase(), m_pModule->GetBase(), m_pModule->GetLastByte(), pattern);
	}
	else
	{
		result = Memoria::FindBlock(m_pModule->GetSegmentRData().Base, m_pModule->GetSegmentRData().Base, m_pModule->GetSegmentRData().LastByte, pattern);
	}

	if (result != nullptr && (bIsStringRef))
	{
		if (bIsStringDeep)
		{
			result = Memoria::FindReference(m_pModule->GetBase(), m_pModule->GetBase(), m_pModule->GetLastByte(), result, 0x68);
		}
		else
		{
			result = Memoria::FindReference(m_pModule->GetSegmentCode().Base, m_pModule->GetSegmentCode().Base, m_pModule->GetSegmentCode().LastByte, result, 0x68);
		}
	}

	SetOutputInternally(result, false);
}

void CSearchPattern::FindWideString(const wchar_t* aValue, bool bIsStringDeep, bool bIsStringRef, bool bIsStringPart)
{
	if (*m_pOutput == nullptr || aValue == nullptr || *aValue == '\0')
	{
		SetOutputInternally({}, false);
		return;
	}

	// TODO: Implement
	SetOutputInternally({}, false);
}

void CSearchPattern::FindRelative(uint16_t aOpcode, int aIndex, bool aBack)
{
	if (*m_pOutput == nullptr)
		return;

	void* result;

	result = Memoria::FindRelative(*m_pOutput, m_pModule->GetSegmentCode().Base, m_pModule->GetSegmentCode().LastByte,
		aOpcode, aIndex, aBack);

	SetOutputInternally(result, false);
}

void CSearchPattern::FindReference()
{
	if (*m_pOutput == nullptr) {
		return;
	}

	void* result;

	result = Memoria::FindReference(m_pModule->GetBase(), m_pModule->GetBase(), m_pModule->GetLastByte(), *m_pOutput);

	SetOutputInternally(result, false);
}

void CSearchPattern::FindSelf()
{
	if (*m_pOutput == nullptr) {
		return;
	}

	void* result;

	result = Memoria::FindReference(m_pModule->GetBase(), m_pModule->GetBase(), m_pModule->GetLastByte(), *m_pOutput);

	SetOutputInternally(result, false);
}

void CSearchPattern::FindCall(int aIndex, bool bJumpIn, bool aBack)
{
	if (*m_pOutput == nullptr)
		return;

	void* result;

	result = Memoria::FindRelative(*m_pOutput, m_pModule->GetSegmentCode().Base, m_pModule->GetSegmentCode().LastByte,
		0xE8, aIndex, aBack);

	if (result != nullptr && bJumpIn)
	{
		result = Memoria::Advance(result, 1);
		result = Memoria::Resolve(result, 4);
	}

	SetOutputInternally(result, false);
}

void CSearchPattern::FindNextCall(bool bJumpIn)
{
	FindCall(0, bJumpIn, false);
}

void CSearchPattern::FindPrevCall(bool bJumpIn)
{
	FindCall(0, bJumpIn, true);
}

void CSearchPattern::JoinCall(int aIndex, bool aBack)
{
	FindCall(aIndex, true, aBack);
}

void CSearchPattern::JoinNextCall()
{
	JoinCall(0, false);
}

void CSearchPattern::JoinPrevCall()
{
	JoinCall(0, true);
}

void CSearchPattern::FindJump(int aIndex, bool bJumpIn, bool aBack)
{
	if (*m_pOutput == nullptr)
		return;

	void* result;

	result = Memoria::FindRelative(*m_pOutput, m_pModule->GetSegmentCode().Base, m_pModule->GetSegmentCode().LastByte,
		0xE9, aIndex, aBack);

	if (result != nullptr && bJumpIn)
	{
		result = Memoria::Advance(result, 1);
		result = Memoria::Resolve(result, 4);
	}

	SetOutputInternally(result, false);
}

void CSearchPattern::FindNextJump(bool bJumpIn)
{
	FindJump(0, bJumpIn, false);
}

void CSearchPattern::FindPrevJump(bool bJumpIn)
{
	FindJump(0, bJumpIn, true);
}

void CSearchPattern::JoinJump(int aIndex, bool aBack)
{
	FindJump(aIndex, true, aBack);
}

void CSearchPattern::JoinNextJump()
{
	JoinJump(0, false);
}

void CSearchPattern::JoinPrevJump()
{
	JoinJump(0, true);
}

void CSearchPattern::FindVTable(const std::string& aName)
{
	if (*m_pOutput == nullptr || aName.empty())
	{
		SetOutputInternally({}, false);
		return;
	}

	auto desc = Memoria::GetRTTIDescriptor(m_pModule->GetSegmentData().Base, m_pModule->GetSegmentData().Base, m_pModule->GetSegmentData().LastByte, aName);

	if (desc == nullptr)
	{
		SetOutputInternally({}, false);
		return;
	}

	void* result;

	result = Memoria::GetVTableForDescriptor(m_pModule->GetBase(), m_pModule->GetBase(), m_pModule->GetLastByte(), desc);

	SetOutputInternally(result, false);
}

bool CSearchPattern::CheckUInt8(uint8_t aValue, int aOffset)
{
	if (*m_pOutput == nullptr) {
		return false;
	}

	return Memoria::CheckUInt8(*m_pOutput, aValue, aOffset);
}

bool CSearchPattern::CheckUInt16(uint16_t aValue, int aOffset)
{
	if (*m_pOutput == nullptr)
		return false;

	return Memoria::CheckUInt16(*m_pOutput, aValue, aOffset);
}

bool CSearchPattern::CheckUInt32(uint32_t aValue, int aOffset)
{
	if (*m_pOutput == nullptr)
		return false;

	return Memoria::CheckUInt32(*m_pOutput, aValue, aOffset);
}

void CSearchPattern::GetExport(const std::string& name)
{
	void* result;

	result = GetProcAddress((HMODULE)m_pModule->GetBase(), name.c_str());

	SetOutputInternally(result, false);
}

void CSearchPattern::GetInterface(const std::string& name)
{
	using CreateInterfaceFunc = void* (*)(const char* name, int* returnCode);
	auto createInterface = (CreateInterfaceFunc)GetProcAddress((HMODULE)m_pModule->GetBase(), "CreateInterface");

	if (createInterface == nullptr)
	{
		SetOutputInternally({}, false);
		return;
	}

	void* result;

	result = createInterface(name.c_str(), nullptr);

	SetOutputInternally(result, false);
}

void CSearchPattern::Dereference()
{
	if (*m_pOutput != nullptr)
		SetOutputInternally(*m_pOutput, true);
}

void CSearchPattern::Resolve()
{
	if (*m_pOutput != nullptr)
	{
		void* result = Memoria::Resolve(*m_pOutput, 0);
		SetOutputInternally(result, false);
	}
}

void CSearchPattern::Resolve(int aOffset)
{
	if (*m_pOutput != nullptr)
	{
		void* result = Memoria::Resolve(*m_pOutput, aOffset);
		SetOutputInternally(result, false);
	}
}

void CSearchPattern::Resolve(int aPreOffset, int aPostOffset)
{
	if (*m_pOutput != nullptr)
	{
		Advance(aPreOffset);
		void* result = Memoria::Resolve(*m_pOutput, aPostOffset);
		SetOutputInternally(result, false);
	}
}

void CSearchPattern::Advance(ptrdiff_t aValue)
{
	if (*m_pOutput == nullptr)
		return;

	void* result;

	result = Memoria::Advance(*m_pOutput, aValue);

	SetOutputInternally(result, false);
}

void CSearchPattern::Add(size_t aValue)
{
	Advance((signed)aValue);
}

void CSearchPattern::Sub(size_t aValue)
{
	Advance(-(signed)aValue);
}

void CSearchPattern::Align(size_t nValue)
{
	if (*m_pOutput != nullptr)
		SetOutputInternally((void*)((intptr_t)(*m_pOutput) & ~(nValue - 1)), false);
}

void CSearchPattern::Reset()
{
	SetOutputInternally(m_pModule->GetBase(), false);
}

void CSearchPattern::Invalidate()
{
	if (m_pOutput)
		*m_pOutput = nullptr;
}

void CSearchPattern::ForceOutput(void* aValue)
{
	if (aValue == nullptr || !Memoria::IsWithinBounds(aValue, m_pModule->GetBase(), m_pModule->GetLastByte()))
	{
		*m_pOutput = nullptr;
		return;
	}

	SetOutputInternally(aValue, false);
}

void* CSearchPattern::CurrentOutput()
{
	if (!m_pOutput)
		return nullptr;

	return *m_pOutput;
}

bool CSearchPattern::IsValid()
{
	if (m_pOutput == nullptr || m_pModule == nullptr)
		return false;

	if (*m_pOutput == nullptr)
		return false;

	if (!Memoria::IsWithinBounds(*m_pOutput, m_pModule->GetBase(), m_pModule->GetLastByte()))
		return false;

	return true;
}

CSearchPattern::CSearchPattern(CModule* aModule, void** aOutput, const std::string& aName)
{
	m_sName = aName;
	m_pModule = aModule;

	if (aOutput != nullptr)
	{
		m_pOutput = (void**)aOutput;
		m_bHasNoOutput = false;
	}
	else
	{
		m_pInternalData = new void*;
		m_pOutput = (void**)m_pInternalData;

		m_bHasNoOutput = true;
	}

	*m_pOutput = m_pModule->GetBase();

#ifdef _DEBUG
	m_pDebugOutput = m_pModule->GetBase();
#endif
}

CSearchPattern::~CSearchPattern()
{
	if (m_bHasNoOutput && m_pInternalData)
		delete m_pInternalData;
}