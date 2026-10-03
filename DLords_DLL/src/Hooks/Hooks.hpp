#pragma once

class CGameHook
{
private:
	std::string m_sName;
	std::string m_sSig;

	void* m_pTarget;
	void* m_pOriginal;
	void* m_pHook;

	bool m_bFound;
	bool m_bResult;

private:
	void CreateHook(void* pfnHook);

public:
	CGameHook() = delete;
	CGameHook(std::string_view sName, std::string_view sSig, void* pfnHook);

	const std::string& GetName() const { return m_sName; }
	const std::string& GetSig() const { return m_sSig; }
	bool IsSuccess() const { return m_bFound && m_bResult; }
	const void* GetTarget() const { return m_pTarget; }
	const void* GetOriginal() const { return m_pOriginal; }
	const void* GetHooked() const { return m_pHook; }

	bool RemoveHook();
	void Log() const;
};

class CGameSearch
{
private:
	std::string m_sName;
	std::string m_sSig;
	int m_iOffset;
	bool m_bFound;
	void* m_pData;

private:
	void Search();

public:
	CGameSearch() = delete;
	CGameSearch(std::string_view name, std::string_view sig, void* pData, int offset = 2);

	bool IsSuccess() const { return m_bFound; }
	const std::string& GetName() const { return m_sName; }
	const std::string& GetSig() const { return m_sSig; }
	void* GetData() const { return m_pData; }

	void Log() const;
};

class CGameHookMgr
{
private:
	static inline std::unique_ptr<std::mutex> m_sMutex{};
	static inline std::unique_ptr<std::list<CGameHook*>> m_sHooks{};
	static inline std::unique_ptr<std::list<CGameSearch*>> m_sSearches{};

	static inline int m_iSearchFound = 0;
	static inline int m_iHookFound = 0;

	CGameHookMgr() = default;
	~CGameHookMgr() = default;

	CGameHookMgr(const CGameHookMgr&) = delete;
	CGameHookMgr& operator=(const CGameHookMgr&) = delete;

	void LogHook() const;
	void LogSearch() const;

public:
	static CGameHookMgr& Instance();

	void push_back(CGameHook* pGameHook);
	void push_back(CGameSearch* pGameSearch);
	void RemoveAllHooks();

	size_t GetHookCount() const;
	size_t GetSearchCount() const;
	void Log() const;
};

#define CREATE_GEH(sig,org,hk)                                                                      \
	static CGameHook CB_COMBINE(GEH ## _ ## hk ## _, __LINE__) = CGameHook(#hk, ##sig, reinterpret_cast<void*>(hk));          \
	org = reinterpret_cast<decltype(org)>(CB_COMBINE(GEH ## _ ## hk ## _, __LINE__).GetOriginal());

#define CREATE_GES(sig,data,offset)                                                                  \
	static CGameSearch CB_COMBINE(GES ## _ ## data ## _, __LINE__) = CGameSearch(#data, ##sig, data, offset); \
	data = reinterpret_cast<decltype(data)>(CB_COMBINE(GES ## _ ## data ## _, __LINE__).GetData());
