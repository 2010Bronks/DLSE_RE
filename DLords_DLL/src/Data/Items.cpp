#include "precompiled.hpp"

static size_t* s_pItemInfoCount = nullptr;
static dl_item_info_t* s_pItemInfoList = nullptr;

CItemHelper::CItemHelper(dl_item_t* pItem) :
	m_pItem(pItem), m_pInfo(nullptr),
	m_isValid(false), m_idx_back(-1),
	m_iLastMod(0)
{
	Init();
}

void CItemHelper::Init()
{
	if (!m_pItem)
	{
		LOG_DBG_WARNING("[CItemHelper::Init] m_pItem == nullptr!");
		m_isValid = false;
		return;
	}

	m_pInfo = ItemInfo::Get(m_pItem->itemIdx);
	if (!m_pInfo)
	{
		LOG_DBG_WARNING("[CItemHelper::Init] m_pInfo == nullptr!");
		m_isValid = false;
		return;
	}

	m_isValid = true;
	m_idx_back = m_pItem->itemIdx;

	for (int i = 0; i < 5; i++)
	{
		if (m_pItem->mod_type[i] != EIM_Type::EIM_None)
		{
			m_iLastMod = i;
			break;
		}
	}
}

void CItemHelper::SetMod(uint32_t modSlot, EIM_Type modType, int16_t modValue)
{
	if (!IsValid())
		return;

	if (modSlot >= 5)
		return;

	m_pItem->mod_type[modSlot] = modType;
	m_pItem->mod_value[modSlot] = modValue;
}

void CItemHelper::AddMod(EIM_Type modType, int16_t modValue)
{
	if (!IsValid())
		return;

	if (m_iLastMod > 4)
		return;

	m_pItem->mod_type[m_iLastMod] = modType;
	m_pItem->mod_value[m_iLastMod] = modValue;

	m_iLastMod++;
}

void CItemHelper::RemoveMod(uint32_t modSlot)
{
	if (!IsValid())
		return;

	if (modSlot >= 5)
		return;

	m_pItem->mod_type[modSlot] = EIM_Type::EIM_None;
	m_pItem->mod_value[modSlot] = 0;
}

void CItemHelper::RemoveLastMod()
{
	if (!IsValid())
		return;

	if (m_iLastMod > 4)
		return;

	m_pItem->mod_type[m_iLastMod] = EIM_Type::EIM_None;
	m_pItem->mod_value[m_iLastMod] = 0;

	m_iLastMod--;
}

void CItemHelper::SetRarity(EIR_Type rarType, int rarMod)
{
	if (!IsValid())
		return;

	m_pItem->rarity.type = rarType;
	m_pItem->rarity.mod = rarMod;
}

void CItemHelper::SetToDefault()
{
	if (!IsValid())
		return;

	m_pItem->itemIdx = m_idx_back;

	m_pItem->flags = m_pInfo->flags;
	m_pItem->durability = m_pInfo->durability;
	m_pItem->count = m_pInfo->count;

	//m_pItem->unk1 = ?;

	m_pItem->rarity = m_pInfo->rarity;

	m_iLastMod = 0;
	for (int i = 0; i < 5; i++)
	{
		m_pItem->mod_type[i] = m_pInfo->mod_type[i];
		m_pItem->mod_value[i] = m_pInfo->mod_value[i];

		if (m_pItem->mod_type[i] != EIM_Type::EIM_None)
		{
			m_iLastMod = i;
		}
	}
}

dl_item_rarity_t* CItemHelper::GetRarity()
{
	if (!IsValid())
		return nullptr;

	return &m_pItem->rarity;
}

int CItemHelper::GetModType(uint32_t modSlot) const
{
	if (!IsValid() || modSlot >= 5)
		return -1;

	return static_cast<int>(m_pItem->mod_type[modSlot]);
}

int CItemHelper::GetModValue(uint32_t modSlot) const
{
	if (!IsValid() || modSlot >= 5)
		return -1;

	return static_cast<int>(m_pItem->mod_value[modSlot]);
}

dl_item_info_t* CItemHelper::GetInfo()
{
	if(!IsValid())
		return nullptr;

	return m_pInfo;
}

std::string CItemHelper::GetName()
{
	if(!IsValid())
		return std::string();

	return m_pInfo->name;
}

void CItemHelper::PrintInfo()
{
	if (!IsValid())
	{
		LOG_DBG_WARNING("[CItemHelper::PrintInfo] Trying to print info, while m_isValid == false!");
		return;
	}

	auto sRarity = m_pItem->rarity.FormatRarity();
	auto sMods = m_pItem->FormatMods();

	LOG("[CItemHelper::PrintInfo] Start");
	{
		LOG("[{}] \"{}\", {} -> {}", m_pItem->itemIdx, m_pInfo->name, m_pInfo->GetTypeStr(), m_pInfo->GetTypeData());
		LOG("Rarity: {}", sRarity);
		for (auto&& s : sMods)
		{
			LOG(s);
		}
	}
	LOG("[CItemHelper::PrintInfo] End");
}

namespace ItemInfo
{
	size_t GetCount()
	{
		if (!s_pItemInfoCount)
			return 0;

		return *s_pItemInfoCount;
	}

	dl_item_info_t* GetList()
	{
		if (!s_pItemInfoList)
			return nullptr;

		return s_pItemInfoList;
	}

	dl_item_info_t* Get(size_t idx)
	{
		auto count = GetCount();
		if (!count)
			return nullptr;

		if (idx == 0 || idx > count)
			return nullptr;

		auto pList = GetList();
		if (!pList)
			return nullptr;

		return &pList[idx];
	}
}

namespace Item
{
	using CreateItem_t = dl_object_t * (__fastcall*)(int itemIdx, vector3* pos, vector3* rot);
	static CreateItem_t pfnCreateItem = nullptr;

	using Item_InitFromIdx_T = void(__fastcall*)(dl_item_t* pItem, int itemIdx, int count);
	static Item_InitFromIdx_T pfnItem_InitFromIdx = nullptr;

	using Item_FormatModRarity_T = void(__fastcall*)(dl_item_t* pItem, int rarModId);
	static Item_FormatModRarity_T pfnItem_FormatModRarity = nullptr;

	using AddItemToPlayer_t = void(__fastcall*)(dl_player_t* pPly, int itemId, int count, int slotId);
	static AddItemToPlayer_t pfnAddItemToPlayer;
	static void __fastcall AddItemToPlayer(dl_player_t* pPly, int itemId, int count, int slotId)
	{
		if (!pfnAddItemToPlayer)
			return;

		auto pItemInfo = ItemInfo::Get(itemId);
		if (pItemInfo)
		{
			LOG("[Item::AddItemToPlayer] Player \"{}\" got {} of \"{}\" in slot {}.", pPly->szName, count, pItemInfo->name, slotId);
		}
		else
		{
			LOG("[Item::AddItemToPlayer] Player \"{}\" got {} of [{}] in slot {}.", pPly->szName, count, itemId, slotId);
		}

		pfnAddItemToPlayer(pPly, itemId, count, slotId);
	}

	using ClearItemsForPlayer_t = void(__fastcall*)(dl_player_t* pPly);
	static ClearItemsForPlayer_t pfnClearItemsForPlayer;
	static void __fastcall ClearItemsForPlayer(dl_player_t* pPly)
	{
		if (!pfnClearItemsForPlayer)
			return;

		LOG("[Item::ClearItemsForPlayer] Requested for \"{}\".", pPly->szName);

		pfnClearItemsForPlayer(pPly);
	}

	int RollRarityMod(int level)
	{
		auto pRnd = Game::GetRandomSeed();
		auto pLcg = Game::GetRandomLcgMul();
		if (!pRnd || !pLcg)
			return 0;

		// Roll rarity
		*pRnd *= *pLcg;
		int v24 = (3 * HIWORD(*pRnd)) >> 16;

		int v25 = level / 5 + v24 - 1;
		if (v25 < 1)
			v25 = 1;

		return v25;
	}

	static dl_object_t* __fastcall CreateItem(int itemIdx, vector3* pos, vector3* rot)
	{
		using namespace Objects;

		auto pObj = pfnCreateItem(itemIdx, pos, rot);

		if (bLog && pObj)
		{
			auto pItemInfo = ItemInfo::Get(itemIdx);
			if (pItemInfo)
			{
				LOG("[CreateItem] Return ObjID[{}] | Args name[{}]", pObj->objID.id, pItemInfo->name);
			}
			else
			{
				LOG("[CreateItem] Return ObjID[{}] | Args itemIdx[{}]", pObj->objID.id, itemIdx);
			}
		}

		return pObj;
	}
	
	void __fastcall Item_InitFromIdx(dl_item_t* pItem, int itemIdx, int count)
	{
		if (!pfnItem_InitFromIdx)
			return;

		pfnItem_InitFromIdx(pItem, itemIdx, count);
	}

	void __fastcall Item_FormatModRarity(dl_item_t* pItem, int rarModId)
	{
		if (!pfnItem_FormatModRarity)
			return;

		pfnItem_FormatModRarity(pItem, rarModId);
	}

	using Item_SpawnInWorld_T = int(__fastcall*)(int itemIdx, vector3* pos, int a3, dl_item_t* pItem);
	static Item_SpawnInWorld_T pfnItem_SpawnInWorld = nullptr;
	static int __fastcall Item_SpawnInWorld(int itemIdx, vector3* pos, int a3, dl_item_t* pItem)
	{
		if (!pfnItem_SpawnInWorld)
			return -1;

		return pfnItem_SpawnInWorld(itemIdx, pos, a3, pItem);
	}

	bool AddToPlayer(dl_player_t* pPly, uint32_t itemId, uint32_t count, int32_t slotId)
	{
		if (!pfnAddItemToPlayer)
		{
			LOG_ERROR("[Item::AddToPlayer] Function pointer is not ready.");
			return false;
		}

		if (!pPly || !strlen(pPly->szName))
		{
			LOG_ERROR("[Item::AddToPlayer] Invalid Player pointer.");
			return false;
		}

		auto itemsCount = ItemInfo::GetCount();
		if (!itemsCount)
		{
			LOG_ERROR("[Item::AddToPlayer] Could not get items count[{}].", itemsCount);
			return false;
		}

		if (itemId == 0 || itemId > itemsCount)
		{
			LOG_ERROR("[Item::AddToPlayer] Invalid item id[{}].", itemId);
			return false;
		}

		pfnAddItemToPlayer(pPly, itemId, count, slotId);
		return true;
	}

	bool ClearAllForPlayer(dl_player_t* pPly)
	{
		if (!pfnClearItemsForPlayer)
		{
			LOG_ERROR("[Item::ClearAllForPlayer] Function pointer is not ready.");
			return false;
		}

		if (!pPly || !strlen(pPly->szName))
		{
			LOG_ERROR("[Item::ClearAllForPlayer] Invalid Player pointer.");
			return false;
		}

		pfnClearItemsForPlayer(pPly);
		return true;
	}

	bool SpawnInWorld(uint32_t itemId, int level)
	{
		if (!pfnItem_SpawnInWorld)
		{
			LOG_ERROR("[Item::SpawnInWorld] Function pointer is not ready.");
			return false;
		}

		auto pLocal = GetLocalEntity();
		if (!pLocal)
		{
			LOG_ERROR("[Item::SpawnInWorld] Local Player pointer is not ready.");
			return false;
		}

		if (itemId == 0 || itemId > ItemInfo::GetCount())
		{
			LOG_ERROR("[Item::SpawnInWorld] Invalid itemId[{}].", itemId);
			return false;
		}

#if 0
		dl_item_t pItem;

		int rar = 0; 
		if (auto pPly = GetLocalPlayer())
			rar = RollRarityMod(level == 0 ? pPly->GetData()->iLevel32 : level);
		else
			rar = RollRarityMod(level);

		pfnItem_InitFromIdx(&pItem, itemId, 0);
		pfnItem_FormatModRarity(&pItem, rar);
#endif
	
		vector3 forward, rot;
		rot = ToSourceAngles(pLocal->rot);
		GetDirections(&rot, &forward);
		auto pos = pLocal->pos + forward * 4.0f * 1024.f;

		auto pObj = CreateItem(itemId, &pos, &rot);
		return pObj ? true : false;
	}

	static void Find()
	{
		CREATE_GEH("E8 ?? ?? ?? ?? 68 84 00 00 00 6A 00 BA 68 01 00 00 8B CE E8 ?? ?? ?? ?? 6A 78", pfnAddItemToPlayer, AddItemToPlayer);
		CREATE_GEH("E8 ?? ?? ?? ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 8E 0C 83 00 00", pfnClearItemsForPlayer, ClearItemsForPlayer);
		CREATE_GEH("E8 ?? ?? ?? ?? 89 06 EB 1C", pfnCreateItem, CreateItem);
		CREATE_GEH("E8 ?? ?? ?? ?? 8B 44 24 10 89 44 24 38", pfnItem_InitFromIdx, Item_InitFromIdx);
		CREATE_GEH("E8 ?? ?? ?? ?? 8D 4C 24 28 51 8B 4C 24 14", pfnItem_FormatModRarity, Item_FormatModRarity);
		CREATE_GEH("E8 ?? ?? ?? ?? 5F 5E 5B 8B 4C 24 30", pfnItem_SpawnInWorld, Item_SpawnInWorld);
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 E8 ?? ?? ?? ?? E8", s_pItemInfoCount, 1);
	CREATE_GES("68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 0F B7 0D", s_pItemInfoList, 1);

	auto item_info_list = gGameModule->Sig().FindSignature("68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 0F B7 0D").Add(1).Dereference().Sub(sizeof(dl_item_info_t));
	s_pItemInfoList = reinterpret_cast<decltype(s_pItemInfoList)>(item_info_list.Get());
	if (!item_info_list.IsValid())
		LOG_IMPORTANT("[ItemInfoList] Could not find signature!");

	Item::Find();

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);