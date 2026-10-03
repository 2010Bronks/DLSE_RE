#include "precompiled.hpp"

FallDam_t pfnFallDam;

/*
	You can kill anyone just by calling this function with spoofed id and dmg!
*/
static void __fastcall FallDam(int id, int dmg, int flags, int a4, vector3* pos)
{
	//LOG("[FallDam] id[{}] dmg[{}] a3[{}] a4[{}] pos: {}, {}, {}", id, dmg, flags, a4, pos->x, pos->y, pos->z);

	//id = 20;
	//dmg = 1000;

	pfnFallDam(id, dmg, flags, a4, pos);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ? ? ? ? 8B 86 ? ? ? ? 89 86 ? ? ? ? 83 7C 24", pfnFallDam, FallDam);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);