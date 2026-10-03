#pragma once

#include <vector>

#include "callbacks_args.hpp"

using HookChain_Init = HookChain<void, CHookChainArgs_Init &>;
using HookChain_Done = HookChain<void, CHookChainArgs_Done &>;
using HookChain_OnMenuBuild = HookChain<void, CHookChainArgs_OnMenuBuild &>;
using HookChain_OnMenuInit = HookChain<void, CHookChainArgs_OnMenuInit &>;
using HookChain_OnDrawUI = HookChain<void, CHookChainArgs_OnDrawUI &>;
using HookChain_OnDllFrame = HookChain<void, CHookChainArgs_OnDllFrame&>;

using HookChain_OnGameFrame = HookChain<void, CHookChainArgs_OnGameFrame&>;
using HookChain_OnMonsterSpawn = HookChain<void, CHookChainArgs_OnMonsterSpawn&>;
using HookChain_OnGuiPageChange = HookChain<void, CHookChainArgs_OnGuiPageChange&>;

#define DECLARE_CALLBACK_MGR_METHOD(name) \
	static HookChain_##name::CCallbackRegistry& name();

#define IMPLEMENT_CALLBACK_MGR_METHOD(name) \
	HookChain_##name::CCallbackRegistry& CCallbackMgr::name() { \
		static HookChain_##name::CCallbackRegistry callchain{}; \
		static bool registered = (_registry.emplace_back(#name, callchain), true); \
		return callchain; \
	}

class CCallbackMgr
{
private:
	static inline std::vector<std::pair<std::string, AbstractHookChainRegistry &>> _registry;

public:
	static const auto &GetRegistry()
	{
		return _registry;
	}

	DECLARE_CALLBACK_MGR_METHOD(Init);
	DECLARE_CALLBACK_MGR_METHOD(Done);
	DECLARE_CALLBACK_MGR_METHOD(OnMenuBuild);
	DECLARE_CALLBACK_MGR_METHOD(OnMenuInit);
	DECLARE_CALLBACK_MGR_METHOD(OnDrawUI);
	DECLARE_CALLBACK_MGR_METHOD(OnDllFrame);
	DECLARE_CALLBACK_MGR_METHOD(OnGameFrame);
	DECLARE_CALLBACK_MGR_METHOD(OnMonsterSpawn);
	DECLARE_CALLBACK_MGR_METHOD(OnGuiPageChange);
};