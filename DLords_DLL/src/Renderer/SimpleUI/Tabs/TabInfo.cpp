#include "precompiled.hpp"

extern CSimpleUI gSimpleUI;

static constexpr auto s_tab_name = "Info";
static bool s_bGameDebug = false;
static bool s_bShowCoords = false;

extern void RequestUnload();
static void TabElements()
{
	auto& style = ImGui::GetStyle();
	
	SimpleUI::Checkbox("Game logs", &GameDebug::bLog);

	if (SimpleUI::Checkbox("Game dbg", &s_bGameDebug))
	{
		GameDebug::SetDebugState(s_bGameDebug);
	}

	if (SimpleUI::Checkbox("Show coords", &s_bShowCoords))
	{
		GameDebug::SetShowCoords(s_bShowCoords);
	}

	SimpleUI::Checkbox("Log UI Pages", &GameUI::bLogPageOpen);

	if (ImGui::Button("Log Binds"))
	{
		Binds::LogAll();
	}

	if (ImGui::Button("Log CMDs"))
	{
		Commands::LogAll();
	}

	if (ImGui::Button("Log ItemInfo"))
	{
		auto itemCount = ItemInfo::GetCount();
		if (itemCount == 0)
			return;

		if (auto pItems = ItemInfo::GetList())
		{
			LOG("========= LOGGING ITEMS =========");
			for (auto i = 0u; i < itemCount; i++)
			{
				auto& pItem = pItems[i];
				LOG("[{}] {}: [{}] [{}]", i, pItem.name, pItem.GetTypeStr(), pItem.GetTypeData());
			}
			LOG("============= DONE ==============");
		}
	}

	if (ImGui::Button("Log Player Items"))
	{
		auto pLocal = GetPlayerData(0);
		if (!pLocal)
			return;

		auto pSlot = pLocal->GetInventory(EInvSlot::eWeapons);
		if (!pSlot)
			return;

		LOG("========= LOGGING Player Items =========");
		for (auto&& item : pSlot->slot)
		{
			if (item.itemIdx == 0)
				continue;

			auto itemHelper = CItemHelper(&item);
			itemHelper.PrintInfo();
		}
		LOG("================= DONE =================");
	}

	if (ImGui::Button("Log Spells"))
	{
		auto iSpellCount = Spells::GetSpellCount();
		if (iSpellCount > 0)
		{
			if (auto pList = Spells::GetSpellList())
			{
				LOG_DBG("========= LOG_SPELLS_START =========");
				for (auto i = 0u; i < iSpellCount; i++)
				{
					auto& spell = pList[i];
					LOG_DBG("[{}]{} t[{}] lvl[{}] u2[{}] u5[{}] u6[{}] u7[{}]", spell.spellId, spell.name, spell.type, spell.levelNeed, spell.unk2, spell.unk5, spell.unk6, spell.unk7);

				}
				LOG_DBG("========== LOG_SPELLS_END ==========");
			}
		}
	}

	if (ImGui::Button("Unload"))
	{
		RequestUnload();
	}

	static int entId = 8;
	ImGui::InputInt("EntID", &entId);
	if (ImGui::Button("LogEnts"))
	{
		auto count = GetEntityCount();
		if (count > 0)
		{
			for (auto i = 0u; i < count; i++)
			{
				auto pEnt = GetEntityByIndex(i);
				if (!pEnt || !pEnt->pMonster)
					continue;

				LOG("[{}] {}", i, pEnt->pMonster->name);
			}
		}
	}

	if (auto pEnt = GetEntityByIndex(entId))
	{
		if (auto pMon = pEnt->pMonster)
		{
			ImGui::Text(pMon->name);
			if (ImGui::Button("Log Inv"))
			{
				LOG("[{}] Inventory:", pEnt->pMonster->name);
				for (int i = 0; i < 10; i++)
				{
					auto& item = pEnt->inventory[i];
					if (item.itemIdx <= 0 || item.itemIdx > 955)
						continue;

					auto itemInfo = ItemInfo::Get(item.itemIdx);
					if (!itemInfo)
						continue;

					LOG("\t[{}] {}[{}] c[{}] p[{}] t[{}]", i, itemInfo->name, item.itemIdx, itemInfo->count, itemInfo->price, itemInfo->type);
				}
				for (int i = 0; i < 6; i++)
				{
					auto& item = pEnt->inv_loot[i];
					if (item.itemIdx <= 0 || item.itemIdx > 955)
						continue;

					auto itemInfo = ItemInfo::Get(item.itemIdx);
					if (!itemInfo)
						continue;

					LOG("\t[{}] {}[{}] c[{}] p[{}] t[{}]", i, itemInfo->name, item.itemIdx, itemInfo->count, itemInfo->price, itemInfo->type);
				}
				LOG("\n", pEnt->pMonster->name);
			}
		}
	}
}

static void OnMenuInit(HookChain_OnMenuInit::ICallback* chain, CHookChainArgs_OnMenuInit& args)
{
	gSimpleUI.RegisterTabCallback(s_tab_name, TabElements);

	chain->callNext(args);
}

REGISTER_CALLBACK(OnMenuInit, OnMenuInit);