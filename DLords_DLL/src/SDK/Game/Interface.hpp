#pragma once

enum EGuiTexs : uint32_t
{
	eGE_ActionBar = 0,
	eGE_MasterV0Page1 = 1,
	eGE_MasterV0Page2 = 2,
	eGE_MasterV0Page3 = 3,
	eGE_Inventory = 4,
	eGE_ActionBarButtons = 5,
	eGE_CastTime = 6,
	eGE_CoolDown = 7,
	eGE_Equip = 8,
	eGE_Spellbook = 9,
	eGE_ClassSkill = 10,
	eGE_InfoFrame = 11,
	eGE_CreatePage = 12,
	eGE_PopUp = 13,
	eGE_CharMap = 14,
	eGE_ShopPage = 15,
	eGE_TalkLexicon = 16,
	eGE_LootBag = 17,
	eGE_Quantity = 18,
	eGE_Minimap = 19,
	eGE_PCPointer = 20,
	eGE_MapPage = 21,
	eGE_QuestLog = 22,
	eGE_QuestPopup = 23,
	eGE_DisarmTrap = 24,
	eGE_MainMenu2015 = 25,
	eGE_MainMenu2012Button = 26,
	eGE_Heraldry = 27,
	eGE_LoadSave = 28,
	eGE_MoonBridge = 29,
	eGE_Config = 30,
	eGE_Keyboard = 31,
	eGE_LanConnect = 32,
	eGE_IpConnect = 33,
	eGE_SteamLobby = 34,
	eGE_MultiPlayer = 35,
	eGE_CreateBackdrop = 36,
	eGE_CreateBackdropW = 37,
	eGE_OutsideFargrove = 38,
	eGE_SceneLoadBorder = 39,

	eGE_Max
};

enum EGuiPages : uint32_t
{
	eGP_Equip = 0,
	eGP_Magic = 1,
	eGP_Stats = 2,
	eGP_Quests = 3,
	eGP_Map = 4,
	eGP_Menu = 5,

	eGP_Max
};

static const std::string GuiPageToStr(EGuiPages num)
{
#define PACKET_NUM_CASE(name) case EGuiPages::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(eGP_Equip);
		PACKET_NUM_CASE(eGP_Magic);
		PACKET_NUM_CASE(eGP_Stats);
		PACKET_NUM_CASE(eGP_Quests);
		PACKET_NUM_CASE(eGP_Map);
		PACKET_NUM_CASE(eGP_Menu);

	default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

enum ESpellBookPages : uint32_t
{
	ESBP_General,
	ESBP_Arcane,
	ESBP_Crystal,
	ESBP_Nether,
	ESBP_Rune,
	ESBP_NetherKatals,
};

using GUI_OpenPage_t = void(__fastcall*)(EGuiPages pageId); // __fastcall?