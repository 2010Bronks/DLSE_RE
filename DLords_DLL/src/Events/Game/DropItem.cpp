#include "precompiled.hpp"

DropItem_t pfnDropItem;

// slotId == -1 means "TAKE ALL"
static void __fastcall DropItem(int slotId, vector3* pos, dl_item_t* pItem)
{
	auto item = CItemHelper(pItem);
	if (item.IsValid())
	{
		LOG("[DropItem] slotId[{}] pItem[{}]", slotId, item.GetName());
	}
	else
	{
		LOG("[DropItem] slotId[{}] pItem: idx[{}]", slotId, pItem->itemIdx);
	}
	

	pfnDropItem(slotId, pos, pItem);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ?? ?? ?? ?? 83 C8 FF 5F 5E 5D 5B 8B 4C 24 50", pfnDropItem, DropItem);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);