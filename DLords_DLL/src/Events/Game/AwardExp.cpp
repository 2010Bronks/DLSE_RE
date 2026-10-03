#include "precompiled.hpp"

AwardExp_t pfnAwardExp;

static void __fastcall AwardExp(int plyId, int exp)
{
	LOG("[AwardExp] Player[{}] rewarded with [{}] experience.", plyId, exp);

	//exp *= 1000;

	pfnAwardExp(plyId, exp);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ? ? ? ? D9 E8 D9 05", pfnAwardExp, AwardExp);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);