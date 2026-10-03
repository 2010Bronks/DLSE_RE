#include "precompiled.hpp"

bool gbReduceCD = false;

static void ClearSlotCD(dl_item_t& slot)
{
	if (slot.itemIdx <= 0)
		return;

	if (slot.maxCooldown <= 0 && slot.curCooldown <= 0)
		return;

	slot.curCooldown = 0;
}

static void EveryFrame()
{
	if (!gbReduceCD)
		return;

	auto pLocal = GetLocalPlayer();
	if (!pLocal || !pLocal->GetData())
		return;

	auto pInv = &pLocal->GetData()->inventory;
	if (!pInv)
		return;

	for (auto& slot : pInv->weapons.slot)
	{
		ClearSlotCD(slot);
	}

	for (auto& slot : pInv->armor.slot)
	{
		ClearSlotCD(slot);
	}

	for (auto& slot : pInv->jewerly.slot)
	{
		ClearSlotCD(slot);
	}

	for (auto& slot : pInv->potions_scrolls.slot)
	{
		ClearSlotCD(slot);
	}

	for (auto& slot : pInv->keys_quest.slot)
	{
		ClearSlotCD(slot);
	}

	for (auto& slot : pInv->junk_bag.slot)
	{
		ClearSlotCD(slot);
	}

	for (auto& slot : pInv->other)
	{
		ClearSlotCD(slot);
	}
}

static void OnGameFrame(HookChain_OnGameFrame::ICallback* chain, CHookChainArgs_OnGameFrame& args)
{
	EveryFrame();
	chain->callNext(args);
}

REGISTER_CALLBACK(OnGameFrame, OnGameFrame);