#include "precompiled.hpp"

static CPlayer s_Local{};
static CPlayer s_Players[8]{};

static dl_player_t* s_pPlayers = nullptr; // Player::Init
static dl_player_t* s_pLocalPlayer = nullptr;

CPlayer* GetPlayerData(int plyId)
{
	return &s_Players[plyId];
}

CPlayer* GetLocalPlayer()
{
	return &s_Players[0];
}

static void EveryFrame()
{
	if (!s_pPlayers)
	{
		for (int i = 0; i < 8; i++)
		{
			auto& ply = s_Players[i];

			if (ply.IsValid() || ply.GetData())
				ply.Update(nullptr);
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			s_Players[i].Update(&s_pPlayers[i]);
		}
	}

	if (!s_pLocalPlayer)
	{
		s_Local.Update(nullptr);
	}
	else
	{
		s_Local.Update(s_pLocalPlayer);
	}
}

static void OnGameFrame(HookChain_OnGameFrame::ICallback* chain, CHookChainArgs_OnGameFrame& args)
{
	EveryFrame();

	chain->callNext(args);
}

extern bool g_bAddLevel;
using GetNextPlayerLevel_t = int(__fastcall*)(dl_player_t* pPly);
static GetNextPlayerLevel_t pfnGetNextPlayerLevel = nullptr;
static int __fastcall GetNextPlayerLevel(dl_player_t* pPly)
{
	if (!pfnGetNextPlayerLevel)
		return 1;

	auto res = pfnGetNextPlayerLevel(pPly);

	//LOG("[GetNextPlayerLevel] res = {}", res);

	if (g_bAddLevel)
	{
		//LOG("[GetNextPlayerLevel] trying to add level");
		g_bAddLevel = false;
		res++;
		if (auto pPly = GetLocalPlayer())
		{
			if (auto pData = pPly->GetData())
			{
				pData->iExpPoints += pData->iExpPoints2;
				pData->iExpPoints2 = 0;
			}
		}
	}

	return res;
}

using UpdatePlayerStats_t = void(__fastcall*)(int plyId);
static UpdatePlayerStats_t pfnUpdatePlayerStats = nullptr;
static void __fastcall UpdatePlayerStats(int plyId)
{
	if (!pfnUpdatePlayerStats)
		return;

	pfnUpdatePlayerStats(plyId);
	//LOG_IMPORTANT("Player[{}] just leveled up!", plyId);
}

using IsHeraldyActive_t = BOOL(__fastcall*)(dl_player_t* pPly, int heraldyId);
static IsHeraldyActive_t pfnIsHeraldyActive = nullptr;
static BOOL __fastcall IsHeraldyActive(dl_player_t* pPly, int heraldyId)
{
	if (!pfnIsHeraldyActive)
		return 0;

	BOOL res = pfnIsHeraldyActive(pPly, heraldyId);
	//LOG_IMPORTANT("{}: heraldy[{}] exists for player? {}", pPly->szName, heraldyId, res > 0 ? "yes" : "no");
	return res;
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("81 C1 ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 F8 ? 74", s_pPlayers, 2);
	CREATE_GES("68 ? ? ? ? E8 ? ? ? ? 8B 15 ? ? ? ? 83 C4 ? 33 C9", s_pLocalPlayer, 1);
	CREATE_GEH("E8 ?? ?? ?? ?? 80 3D ?? ?? ?? ?? ?? 74 15", pfnUpdatePlayerStats, UpdatePlayerStats);
	CREATE_GEH("E8 ?? ?? ?? ?? 0F BF C8 69 C9 E8 03 00 00", pfnGetNextPlayerLevel, GetNextPlayerLevel);
	CREATE_GEH("E8 ?? ?? ?? ?? 85 C0 74 05 BE 0A 00 00 00 BA 21 00 00 00", pfnIsHeraldyActive, IsHeraldyActive);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);
REGISTER_CALLBACK(OnGameFrame, OnGameFrame, HC_PRIORITY_UNINTERRUPTABLE);

void CPlayer::SetBar(stat_bar_t* bar, int16_t value)
{
	if (!bar)
		return;

	bar->max = value;
	bar->cur = value;
}

void CPlayer::RestoreBar(stat_bar_t* bar, int16_t amount)
{
	if (!bar)
		return;

	if (amount <= 0)
	{
		bar->cur = bar->max;
		return;
	}

	if (bar->cur < bar->max)
		bar->cur = std::clamp(static_cast<int16_t>(bar->cur + amount), static_cast<int16_t>(0), bar->max);
}

void CPlayer::Invalidate()
{
	m_isValid = false;
	m_pData = nullptr;
	m_sName.clear();
}

CPlayer::CPlayer() :
	m_isValid(false), m_pData(nullptr), m_sName{}
{

}

void CPlayer::Update(dl_player_t* m_pData)
{
	if (!m_pData)
		return Invalidate();

	this->m_pData = m_pData;
	this->m_isValid = (this->m_pData) && (strlen(this->m_pData->szName) > 0);

	if (!this->m_isValid)
		return Invalidate();

	m_sName = std::format("{}", this->m_pData->szName);
}

const bool CPlayer::IsValid() const
{
	return m_isValid;
}

void CPlayer::SetHealth(int16_t value)
{
	if (!m_pData)
		return;

	SetBar(&m_pData->Health, value);
}

void CPlayer::SetMana(int16_t value)
{
	if (!m_pData)
		return;

	SetBar(&m_pData->Mana, value);
}

void CPlayer::RestoreHealth(int16_t amount)
{
	if (!IsValid())
		return;

	RestoreBar(&m_pData->Health, amount);
}

void CPlayer::RestoreMana(int16_t amount)
{
	if (!IsValid())
		return;

	RestoreBar(&m_pData->Mana, amount);
}

int16_t CPlayer::GetHealth()
{
	if (!IsValid())
		return 0;

	return m_pData->Health.cur;
}

int16_t CPlayer::GetHealthMax()
{
	if (!IsValid())
		return 0;

	return m_pData->Health.max;
}

int16_t CPlayer::GetMana()
{
	if (!IsValid())
		return 0;

	return m_pData->Mana.cur;
}

int16_t CPlayer::GetManaMax()
{
	if (!IsValid())
		return 0;

	return m_pData->Mana.max;
}

const std::string& CPlayer::GetName() const
{
	return m_sName;
}

dl_player_t* CPlayer::GetData()
{
	return m_pData;
}

dl_player_inventory_t* CPlayer::GetInventory(EInvSlot eSlot)
{
	if (!IsValid())
		return nullptr;

	switch (eSlot)
	{
	default:
	case EInvSlot::eWeapons:
		return &m_pData->inventory.weapons;
		break;

	case EInvSlot::eArmor:
		return &m_pData->inventory.armor;
		break;

	case EInvSlot::eJewerly:
		return &m_pData->inventory.jewerly;
		break;

	case EInvSlot::ePotionsScrolls:
		return &m_pData->inventory.potions_scrolls;
		break;

	case EInvSlot::eKeysQuest:
		return &m_pData->inventory.keys_quest;
		break;

	case EInvSlot::eJunkBag:
		return &m_pData->inventory.junk_bag;
		break;
	}
}

dl_on_player_items_t* CPlayer::GetPlayerItems()
{
	if (!IsValid())
		return nullptr;

	return &m_pData->inventory.on_player;
}

dl_item_t* CPlayer::GetOtherItems(size_t iSlot)
{
	if (!IsValid())
		return nullptr;

	if (iSlot >= 275)
		return nullptr;

	return &m_pData->inventory.other[iSlot];
}

bool CPlayer::IsHeraldyActive(int id)
{
	auto pData = GetData();
	if (!pData)
		return false;

	int arrayIndex = id / 32 + 14;
	int bitOffset = id % 32;

	return (pData->heraldy[arrayIndex] & (1 << bitOffset)) != 0;
}

void CPlayer::SetHeraldy(int id, bool value)
{
	auto pData = GetData();
	if (!pData)
		return;

	int arrayIndex = id / 32 + 14;
	int bitMask = 1 << (id % 32);

	if (value)
		pData->heraldy[arrayIndex] |= bitMask;
	else
		pData->heraldy[arrayIndex] &= ~bitMask;
}
