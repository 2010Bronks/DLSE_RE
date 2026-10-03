#include "precompiled.hpp"

namespace GameMap
{
	static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args);

	static GameMap_t* s_pGameMap = nullptr;
}

namespace GameMap
{
	GameMap_t* Get()
	{
		return s_pGameMap ? s_pGameMap : nullptr;
	}
}

static void GameMap::Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 8C 24 10 01 00 00", s_pGameMap, 1);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, GameMap::Init);