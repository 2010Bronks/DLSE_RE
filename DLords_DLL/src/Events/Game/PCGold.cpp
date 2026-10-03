#include "precompiled.hpp"

PCGold_t pfnPCGold;

static void __fastcall PCGold(int plyId, int gold)
{
	LOG("[PCGold] Player[{}] rewarded with [{}] gold.", plyId, gold);

	//gold *= 1000;

	pfnPCGold(plyId, gold);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ? ? ? ? 8B CB E8 ? ? ? ? 50", pfnPCGold, PCGold);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);