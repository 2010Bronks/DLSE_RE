#include "precompiled.hpp"

CGameHook::CGameHook(std::string_view sName, std::string_view sSig, void* pfnHook) :
	m_sName(sName), m_sSig(sSig), m_pTarget(nullptr), m_pOriginal(nullptr), m_pHook(nullptr), m_bFound(false), m_bResult(false)
{
	if (m_sName.empty())
	{
		LOG_WARNING("[CGameHook::Ctor] Trying to create hook without name.");
		assert(false);
		return;
	}

	if (sSig.empty())
	{
		LOG_WARNING("[CGameHook::Ctor][{}] Trying to create hook without sig.", m_sName);
		assert(false);
		return;
	}

	if (!pfnHook)
	{
		LOG_WARNING("[CGameHook::Ctor][{}] Invalid pfnHook.", m_sName);
		assert(false);
		return;
	}

	CreateHook(pfnHook);
}

void CGameHook::Log() const
{
	if (!m_bFound)
	{
		LOG_ERROR("[CGameHook] [{}]: Could not find signature: \"{}\"", m_sName, m_sSig);
		return;
	}

	if (!m_bResult)
	{
		LOG_ERROR("[CGameHook] [{}]: Could not create hook.", m_sName);
		return;
	}

	LOG_SUCCESS("[CGameHook] [{}]: Found and hooked.", m_sName);
}

void CGameHook::CreateHook(void* pfnHook)
{
	auto pattern = gGameModule->Sig().FindSignature(m_sSig).Resolve(1, 4);
	m_pTarget = pattern.Get();
	m_bFound = pattern.IsValid();

	if (m_bFound && U::Memory::Splice(m_pTarget, pfnHook, &m_pOriginal, false))
	{
		m_bResult = true;
		m_pHook = pfnHook;
	}

	CGameHookMgr::Instance().push_back(this);
}

bool CGameHook::RemoveHook()
{
	if (!m_bResult || !m_pOriginal)
		return false;

	if (!U::Memory::Unsplice(m_pOriginal))
		return false;

	m_pOriginal = nullptr;
	m_pHook = nullptr;
	m_bResult = false;
	return true;
}

CGameSearch::CGameSearch(std::string_view name, std::string_view sig, void* pData, int offset) :
	m_sName(name), m_sSig(sig), m_iOffset(offset), m_bFound(false), m_pData(pData)
{
	if (m_sName.empty())
	{
		LOG_WARNING("[CGameSearch::Ctor] Trying to create search without name.");
		assert(false);
		return;
	}

	if (m_sSig.empty())
	{
		LOG_WARNING("[CGameSearch::Ctor][{}] Trying to create search without sig.", m_sName);
		assert(false);
		return;
	}

	Search();
}

void CGameSearch::Log() const
{
	if (!m_bFound)
		LOG_ERROR("[CGameSearch] [{}]: Could not find signature: \"{}\"", m_sName, m_sSig);
	else
		LOG_SUCCESS("[CGameSearch] [{}]: Found.", m_sName);
}

void CGameSearch::Search()
{
	auto pattern = gGameModule->Sig().FindSignature(m_sSig);
	if (m_iOffset > 0)
		pattern.Add(static_cast<size_t>(m_iOffset));
	else if (m_iOffset < 0)
		pattern.Sub(static_cast<size_t>(-m_iOffset));
	pattern.Dereference();

	m_pData = pattern.Get();
	m_bFound = pattern.IsValid();
	CGameHookMgr::Instance().push_back(this);
}

CGameHookMgr& CGameHookMgr::Instance()
{
	if (!m_sMutex)
		m_sMutex = std::make_unique<std::mutex>();

	static CGameHookMgr instance;
	return instance;
}

void CGameHookMgr::push_back(CGameHook* pGameHook)
{
	std::scoped_lock lock(*m_sMutex);
	if (!m_sHooks)
		m_sHooks = std::make_unique<std::list<CGameHook*>>();
	m_sHooks->push_back(pGameHook);
}

void CGameHookMgr::push_back(CGameSearch* pGameSearch)
{
	std::scoped_lock lock(*m_sMutex);
	if (!m_sSearches)
		m_sSearches = std::make_unique<std::list<CGameSearch*>>();

	for (auto pCur : *m_sSearches)
	{
		if (pCur->GetData() == pGameSearch->GetData())
			LOG_WARNING("[CGameHookMgr::push_back] Addr already presented in arr[{}], search[{}]", pCur->GetName(), pGameSearch->GetName());
	}

	m_sSearches->push_back(pGameSearch);
}

void CGameHookMgr::RemoveAllHooks()
{
	std::scoped_lock lock(*m_sMutex);
	if (!m_sHooks)
		return;

	for (auto it = m_sHooks->rbegin(); it != m_sHooks->rend(); ++it)
	{
		if (*it && (*it)->IsSuccess())
			(*it)->RemoveHook();
	}
}

size_t CGameHookMgr::GetHookCount() const { return m_sHooks ? m_sHooks->size() : 0; }
size_t CGameHookMgr::GetSearchCount() const { return m_sSearches ? m_sSearches->size() : 0; }

void CGameHookMgr::LogHook() const
{
	if (!m_sHooks)
	{
		LOG_WARNING("[CGameHookMgr] Hooker is not ready.");
		return;
	}

	m_iHookFound = 0;
	for (auto hook : *m_sHooks)
	{
		if (!hook)
			continue;
		hook->Log();
		if (hook->IsSuccess())
			++m_iHookFound;
	}
}

void CGameHookMgr::LogSearch() const
{
	if (!m_sSearches)
	{
		LOG_WARNING("[CGameHookMgr] Searcher is not ready.");
		return;
	}

	m_iSearchFound = 0;
	for (auto search : *m_sSearches)
	{
		if (!search)
			continue;
		search->Log();
		if (search->IsSuccess())
			++m_iSearchFound;
	}
}

void CGameHookMgr::Log() const
{
	LogSearch();
	LogHook();

	auto search_count = GetSearchCount();
	auto hook_count = GetHookCount();

	if (search_count == static_cast<size_t>(m_iSearchFound))
		LOG_SUCCESS("[CGameHookMgr] Found [{}/{}] pointers.", m_iSearchFound, search_count);
	else
		LOG_WARNING("[CGameHookMgr] Found [{}/{}] pointers.", m_iSearchFound, search_count);

	if (hook_count == static_cast<size_t>(m_iHookFound))
		LOG_SUCCESS("[CGameHookMgr] Hooked [{}/{}] funcs.", m_iHookFound, hook_count);
	else
		LOG_WARNING("[CGameHookMgr] Hooked [{}/{}] funcs.", m_iHookFound, hook_count);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CGameHookMgr::Instance().Log();
	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init, HC_PRIORITY_LOW);
