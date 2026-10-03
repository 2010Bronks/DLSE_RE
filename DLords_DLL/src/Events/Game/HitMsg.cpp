#include "precompiled.hpp"

HitMsg_t pfnHitMsg;

static void __fastcall HitMsg(int attackerId, int victimId, int amount, const char* reason)
{
	LOG("[HitMsg] Attacker[{}] Victim[{}] by [{}] amount, reason: {}", attackerId, victimId, amount, reason ? reason : "<nothing>");

	pfnHitMsg(attackerId, victimId, amount, reason);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ? ? ? ? 0F B7 8E ? ? ? ? 0F B7 86", pfnHitMsg, HitMsg);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);