#pragma once

/*
	Usefull stuff:

	DEEP6::LoadAnimInfo

	E8 ? ? ? ? 8D 47 ? 3D ? ? ? ? 0F 87 ? ? ? ? 0F B6 80 ? ? ? ? FF 24 85 ? ? ? ? 8D 54 24
	void __fastcall GetModelNameByIdx(int idx, char *out_name)


*/

enum EModelId : uint32_t
{
	GDAK_basemodel = 1,

	EMDL_Invalid1 = 2,

	WOTAI_basemodel = 3,
	BOT_basemodel = 4,
	MINIBOT_basemodel = 5,

	MAGE_basemodel = 7,
	BRUTE_basemodel = 8,
	BAT_basemodel = 9,
	MUMMY_basemodel = 10,
	EVDAK_basemodel = 11,

	EMDL_Invalid2 = 12,
	EMDL_Invalid3 = 13,

	ELFArindale_basemodel = 14,
	TOWNGUARD_basemodel = 15,
	DEVILDOG_basemodel = 16,
	GUILDMASTER_basemodel = 17,
	TOWNFEMALEA_basemodel = 18,
	SKELETON_basemodel = 19,
	GOBLIN_basemodel = 20,
	RAT_basemodel = 21,
	THIEF_basemodel = 22,
	INNKEEPER_basemodel = 23,
	CW_basemodel = 24,
	OGRE_basemodel = 25,
	DGHOUL_basemodel_palerot = 26,
	KNIGHTTEMPLAR_UD_basemodel = 27,
	BARROWGUARD_basemodel = 28,
	SLIME_green_basemodel = 29,

	PCHM_basemodel = 30,

	GOBLIN_MAGE_basemodel = 31,

	PCHF_basemodel = 32,
	PCUM_basemodel_pants1 = 33,

	MOONBEAST_basemodel = 34,
	FEMALE_STATUE_basemodel = 35,
	DRAKE_FIRE_basemodel = 36,
	NAGA_QUEEN_basemodel = 37,
	NAGA_basemodel = 38,
	SCORAB_basemodel = 39,
	SWAMPC_basemodel = 40,
	WOLF_basemodel = 41,
	SPIDERA_basemodel1 = 42,
	FUNGUS_basemodel = 43,
	CHUNNELER_basemodel = 44,
	WRAITH_basemodel = 45,
	DUNGEON_CRAWLER_basemodel = 46,
	KNIGHTTEMPLAR_reg_basemodel = 47,
	MINOTAUR_basemodel = 48,

	PCDM_basemodel_pants = 49,

	SLIME_violet_basemodel = 50,
	GIANTSNAKE_basemodel = 51,
	DEAMONLESSER_basemodel = 52,
	KTELFSPIRIT_basemodel = 53,
	ELFFATHIEN_basemodel = 54,
	ELFDRAE_basemodel = 55,
	ELFSPIRITA_basemodel = 56,
	NAGA_basemodel_fire = 57,
	SMALLSNAKE_basemodelBgreen = 58,
	MONSTERFISH_basemodelA = 59,
	GIANTHORNET_basemodel_red = 60,
	GOBLIN_BALLISTA_basemodel = 61,
	PCHM_LORD_GALDRYN = 62,
	watcherA_basemodel = 63,

	PCWM_basemodel_pants1 = 64,
	PCEM_basemodel = 65,
	PCEF_basemodel = 66,

	swamphag_basemodel = 67,
	PCHF_ELLOWYN_basemodel = 68,
	CACODEMON_basemodel1 = 69,
	SUCCUBUS_basemodel = 70,
	PCHM_DRAEDOTH_GUARD_basemodel = 71,
	CACODEMON_basemodel_STATUE = 72,
	BORLOTH_basemodel = 73,

	CACODEMON_basemodel2 = 74,

	PCDT_basemodel_pants = 75,
	PCZM_basemodel_pants1 = 76,

	VARTUGG_basemodel = 77,
	THRALL_GUARD = 78,
	TREEANT_basemodel = 79,
	PCUM_basemodel_pants2 = 80,
	PCWM_basemodel_pants2 = 81,
	PCZM_basemodel_pants2 = 82,
	SOULDEVOURER_basemodel = 83,
	PCHM_Volgar = 84,
	GHOST_basemodel = 85,
	PIRANHA_basemodelA = 86,
	mimic_basemodel = 87,
	mimic_basemodel_keep = 88,
	mimic_basemodel_SR = 89,
	mimic_basemodel_moors = 90,
	PCHM_Ahn_Po = 91,
	PCDM_Aliester = 92,
	PCDM_Augustus = 93,
	PCHM_Black_Knight = 94,
	PCHF_Celestine = 95,
	PCHM_Davenmor = 96,
	PCHF_Deliah_Irons = 97,
	PCEF_Elvithra = 98,
	PCEM_Emindor = 99,
	PCEF_Fathien_Bladewitch = 100,
	PCEM_FathienLords = 101,
	PCWM_Feras_Dhuul = 102,
	PCHF_Gilea = 103,
	PCDM_Grimlok = 104,
	PCDM_Grunmeir = 105,
	PCHF_Hetta = 106,
	KHAD_basemodel = 107,
	PCHF_LadyLoria = 108,
	PCDM_Lord_Barrowgrim = 109,
	PCEM_Lord_Galebriand = 110,
	PCHM_Lord_Graemare = 111,
	PCEM_MageSpirit = 112,
	PCHF_Mara = 113,
	PCWM_Minas_Tau = 114,
	PCEF_Narako = 115,
	PCEM_Nausolaum = 116,
	PCEM_Nivial = 117,
	PCHM_Okatta = 118,
	PCEM_Orlan_Dray = 119,
	PCHM_Paulus = 120,
	PCHF_Rianne = 121,
	PCHF_Sharia = 122,
	SIMON_basemodel = 123,
	PCEF_SirenSpirit_basemodel = 124,
	PCHF_Sister_Dara =  125,
	PCHF_Staroxia = 126,

	EMDL_Invalid4 = 127,

	PCUM_Turvang_Hammer = 128,
	PCHM_UndeadWarrior_basemodel = 129,
	PCHM_Valdane = 130,
	PCEM_Yoshi_Tamaka = 131,
	PCEM_Vetrion = 132,
	PCDT_Primitive = 133,
	PCHM_EvilSamurai = 134,
	PCHM_Master_Thief = 135,
	PCHM_Ninja_Red = 136,
	PCHM_Rothn_Soldier = 137,
	PCZM_Larok_Ma = 138,
	PCHF_WitchEvil = 139,
	PCHM_NinjaMaster = 140,
	PCHM_Shugenja_Evil = 141,
	PCHM_Warlock_Evil = 142,
	PCHM_Warlock_EvilB = 143,
	PCUM_Urgoth_Evil = 144,
	bat_undead_basemodel = 145,
	drake_lich_basemodel = 146,
	dungeon_crawler_basemodel_blue = 147,
	dungeon_crawler_basemodel_palegreen = 148,
	watcher_soulsphere_basemodel = 149,
	watcher_shadow_basemodel = 150,
	moonbeast_basemodel_hellcat = 151,
	ogre_war_basemodel = 152,
	scorab_basemodel_giant = 153,
	wraith_basemodel_blood = 154,
	goblin_mage_flame_basemodel = 155,
	ghost_basemodel_fire = 156,
	ghost_basemodel_shadow = 157,
	PCEM_ElderB_Basemodel = 158,

	SPIDERA_basemodel2 = 159,

	PCHF_DeliahStreet = 160,
	PCHM_Taluk = 161,

	EMDL_MAX = 162,
	// GDAK_basemodel = default for other
};

struct item_model_t
{
	char pad[84];
};
static_assert(sizeof(item_model_t) == 84);

struct model_unk1_t
{
	vector3 unk1;
	vector2 unk2;
};
static_assert(sizeof(model_unk1_t) == 20);

struct model_info_t
{
	char name[64];
	char pad1[16840];
	void* pFileInfo;
	char pad2[2048];
	int32_t Models_count;
	char pad3[16];
	model_unk1_t* pUnk1;
};
static_assert(sizeof(model_info_t) == 18980);