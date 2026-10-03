#include "precompiled.hpp"

ReqLoot_t pfnReqLoot;

// itemId == -1 means "TAKE ALL"
static void __fastcall ReqLoot(int takerId, int monId, int itemId)
{
	auto pInfo = ItemInfo::Get(itemId);
	if (pInfo)
	{
		LOG("[ReqLoot] takerId[{}] monId[{}] item[{}]", takerId, monId, pInfo->name);
	}
	else
	{
		LOG("[ReqLoot] takerId[{}] monId[{}] itemId[{}]", takerId, monId, itemId);
	}

	if (auto pMon = GetEntityByIndex(monId))
	{
		LOG("Mon {} has {} lootcount.", pMon->pMonster->name, pMon->lootCount);

		for (int i = 0; i < pMon->lootCount; i++)
		{
			const auto& lootId = pMon->loot[i];
			if (auto pItemInfo = ItemInfo::Get(lootId))
			{
				LOG("[Loot_{}] {}[{}] {} => {}", i, pItemInfo->name, pItemInfo->count, pItemInfo->GetTypeStr(), pItemInfo->GetTypeData());
			}
			else
			{
				LOG("[{}] empty", i);
			}
		}
	}


	pfnReqLoot(takerId, monId, itemId);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ? ? ? ? E9 ? ? ? ? F7 C1 ? ? ? ? 74 ? 83 FD", pfnReqLoot, ReqLoot);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);