#include "precompiled.hpp"

static dl_entity_t* s_pEntityList = nullptr;
static int* s_pMonCount = nullptr;

using GetDiff_t = float(*)();
static GetDiff_t pfnGetDiff = nullptr;

static rnd_enc_t* s_pRandomEncounter = nullptr;

// void UpdateFrameTime()

static float GetDiff()
{
	if (!pfnGetDiff)
		return 0.65f; // novice

	float val = pfnGetDiff();
	//LOG_DBG("[GetDiff] returned: {}", val);
	return val;
}

size_t GetEntityCount()
{
	if (!s_pMonCount)
		return 0;

	return static_cast<size_t>(*s_pMonCount);
}

dl_entity_t* GetEntityByIndex(size_t idx)
{
	if (!s_pEntityList)
		return nullptr;

	//if (idx <= 7)
	//{
	//	LOG_DBG_IMPORTANT("[GetEntityByIndex] Trying to get entity with player index [{}].", idx);
	//	return nullptr;
	//}

	size_t entCount = GetEntityCount();
	if (entCount == 0)
		return nullptr;

	if (idx >= entCount && entCount > 8)
	{
		LOG_DBG_IMPORTANT("[GetEntityByIndex] idx[{}] >= EntityCount[{}] | MAX_MONSTER[{}].", idx, entCount, DLords::Entity::MAX_MONSTER);
		return nullptr;
	}

	return &s_pEntityList[idx];
}

dl_entity_t* GetLocalEntity()
{
	return GetEntityByIndex(0);
}

rnd_enc_t* GetRandomEncounter()
{
	if (!s_pRandomEncounter)
		return nullptr;

	return s_pRandomEncounter;
}

extern bool ApplyDamage(int targetId, int dmg, int attackerId);
static void EveryFrame()
{
	if (!s_pEntityList)
		return;

	size_t entCount = GetEntityCount();
	if (entCount == -1)
		return;

	//LOG("[LOG_MONSTERS_START]");
	for (size_t i = 8; i < entCount; i++)
	{
		auto pEnt = GetEntityByIndex(i);
		if (!pEnt)
			continue;

		auto pMonster = pEnt->pMonster;
		if (!pMonster)
			continue;
		
		auto pObj = pEnt->pObject;
		if (!pObj)
			continue;

		//if (pEnt->npc != -1)
		{
			if (GetAsyncKeyState(VK_DELETE))
			{
				ApplyDamage(i, 10, 0);
			}
		}

		//LOG("ID[{}] npc[{}] name[{}] monId[{}] level[{}] objId[{}]", i, pEnt->npc, pMonster->name, pMonster->monId, pMonster->level, (int)pObj->objID.id);
	}
	//LOG("[LOG_MONSTERS_END]\n\n");
}

static void OnGameFrame(HookChain_OnGameFrame::ICallback* chain, CHookChainArgs_OnGameFrame& args)
{
	EveryFrame();

	chain->callNext(args);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("39 35 ? ? ? ? 7E ? BF", s_pMonCount, 2);
	auto entity_list = gGameModule->Sig().FindSignature("66 39 B8 ? ? ? ? 0F 84 ? ? ? ? BF").Add(3).Dereference().Sub(8);
	s_pEntityList = reinterpret_cast<decltype(s_pEntityList)>(entity_list.Get());
	if (!entity_list.IsValid())
		LOG_IMPORTANT("[EntityList] Could not find signature!");

	CREATE_GEH("E8 ?? ?? ?? ?? D9 5C 24 04 D9 44 24 04 D9 14 24", pfnGetDiff, GetDiff);

	auto random_encounter = gGameModule->Sig().FindSignature("89 0D ?? ?? ?? ?? EB 29").Add(2).Dereference().Sub(4);
	s_pRandomEncounter = reinterpret_cast<decltype(s_pRandomEncounter)>(random_encounter.Get());
	if (!random_encounter.IsValid())
		LOG_IMPORTANT("[RandomEncounter] Could not find signature!");

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init, HC_PRIORITY_UNINTERRUPTABLE);
REGISTER_CALLBACK(OnGameFrame, OnGameFrame, HC_PRIORITY_UNINTERRUPTABLE);