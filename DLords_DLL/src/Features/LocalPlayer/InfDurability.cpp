#include "precompiled.hpp"

bool gbInfDurability = false;

static ChronoMeter s_updateTimer;

static void RepairSlot(dl_item_t& slot)
{
	if (slot.itemIdx <= 0)
		return;

	auto item = CItemHelper(&slot);
	if (!item.IsValid())
		return;
	
	auto maxDurability = item.GetInfo()->durability;
	if (slot.durability < maxDurability)
		slot.durability = maxDurability;
}

static void EveryFrame()
{
	if (!gbInfDurability)
		return;

	if (!s_updateTimer.has_elapsed(10s))
		return;

	s_updateTimer.reset();

	auto pLocal = GetLocalPlayer();
	if (!pLocal || !pLocal->GetData())
		return;

	auto pInv = &pLocal->GetData()->inventory;
	if (!pInv)
		return;

	for (auto& slot : pInv->weapons.slot)
	{
		RepairSlot(slot);
	}

	for (auto& slot : pInv->armor.slot)
	{
		RepairSlot(slot);
	}

	for (auto& slot : pInv->jewerly.slot)
	{
		RepairSlot(slot);
	}

	for (auto& slot : pInv->potions_scrolls.slot)
	{
		RepairSlot(slot);
	}

	for (auto& slot : pInv->keys_quest.slot)
	{
		RepairSlot(slot);
	}

	for (auto& slot : pInv->junk_bag.slot)
	{
		RepairSlot(slot);
	}

	for (auto& slot : pInv->other)
	{
		RepairSlot(slot);
	}

	RepairSlot(pInv->on_player.helmet);
	RepairSlot(pInv->on_player.charm);
	RepairSlot(pInv->on_player.pauldrons);
	RepairSlot(pInv->on_player.armor);
	RepairSlot(pInv->on_player.belt);
	RepairSlot(pInv->on_player.pants);
	RepairSlot(pInv->on_player.boots);
	RepairSlot(pInv->on_player.gauntlets);
	RepairSlot(pInv->on_player.band);
	RepairSlot(pInv->on_player.ring1);
	RepairSlot(pInv->on_player.ring2);
	RepairSlot(pInv->on_player.weapon);
	RepairSlot(pInv->on_player.shield);
	RepairSlot(pInv->on_player.range);
}

static void OnGameFrame(HookChain_OnGameFrame::ICallback* chain, CHookChainArgs_OnGameFrame& args)
{
	EveryFrame();
	chain->callNext(args);
}

REGISTER_CALLBACK(OnGameFrame, OnGameFrame);