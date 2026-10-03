#pragma once

#include <stdint.h>

#include "Granny/grannyformat.hpp"

#include "Game/Interface.hpp"
#include "Game/Math.hpp"
#include "Game/Net.hpp"
#include "Game/Entity.hpp"
#include "Game/Binds.hpp"
#include "Game/Player.hpp"
#include "Game/GameRenderer.hpp"
#include "Game/Camera.hpp"
#include "Game/Effects.hpp"
#include "BaseModel.hpp"

enum ELangToken : char
{
	English = 'E',
	French = 'F',
	German = 'G',
	Italian = 'I',
	Romanian = 'R',
	Spanish = 'S',
};

enum EGameState
{
	GS_MAIN = 0,
	GS_ONLINE = 1,
	GS_SAVE = 2,		// Not sure

	GS_NEWGAME = 3,

	GS_PLAYING = 4,			// Show intro when player is afk for 60s

	GS_UNK5 = 5,		// Reset state to 0
	GS_UNK6 = 6,		// Reset state to 0
	GS_UNK7 = 7,		// Reset state to 0
	GS_UNK8 = 8,		// Reset state to 0
	GS_UNK13 = 13,		// Reset state to 0

	GS_LOADGAME = 9,
	GS_UNK18 = 18,		// Reset state to 0 + does something with Roaster

	GS_GOTOMENU = 15,
	GS_CREDITS = 16,	// Play credits video
	GS_INTRO = 17,		// Play 3 into videos

	GS_EXIT = 99,		// Exit from the game

	GS_MAX = 255,
};
static const std::string GameStateToStr(EGameState num)
{
#define GS_NUM_CASE(name) case EGameState::name: return #name;

	switch (num)
	{
		GS_NUM_CASE(GS_MAIN);
		GS_NUM_CASE(GS_ONLINE);
		GS_NUM_CASE(GS_SAVE);
		GS_NUM_CASE(GS_NEWGAME);
		GS_NUM_CASE(GS_PLAYING);
		GS_NUM_CASE(GS_UNK5);
		GS_NUM_CASE(GS_UNK6);
		GS_NUM_CASE(GS_UNK7);
		GS_NUM_CASE(GS_UNK8);
		GS_NUM_CASE(GS_UNK13);
		GS_NUM_CASE(GS_LOADGAME);
		GS_NUM_CASE(GS_UNK18);
		GS_NUM_CASE(GS_CREDITS);
		GS_NUM_CASE(GS_INTRO);
		GS_NUM_CASE(GS_EXIT);

	case EGameState::GS_MAX:
	default: 
		return std::format("unk[{}]", (int)num);
	}

#undef GS_NUM_CASE
}

namespace DLords::BSP
{
	constexpr auto MAX_MAP_FACES = 80000;
	constexpr auto MAX_MAP_NODES = 40000;
	constexpr auto MAX_NAV_LOOKUP = 20000;
	constexpr auto MAX_FOUNTAINS = 16;
}

namespace DLords::DEEP6
{
	constexpr auto MAX_EVENTDECL = 1024;
	constexpr auto MAX_TRIGGERS = 512;
	constexpr auto MAX_BOUNDAREAS = 280;
	constexpr auto MAX_SPECIALS = 128;
	constexpr auto MAX_SWITCHES = 255;
	constexpr auto MAX_TRAPS = 256;

	constexpr auto MAX_MONRECS = 256;

	constexpr auto MAX_COMMANDLINE_ARGS = 32;
}

namespace DLords::Net
{
	constexpr auto MAX_QEVENTS = 128;
	constexpr auto MAX_NPC_MESSAGE_SIZE = 1024;
}

namespace DLords::Granny
{
	constexpr auto ITEMMDL_NUMOF = 1216;
	constexpr auto DAKGRANNY_MAX_ITEMMODELS = 1240;
	constexpr auto DAKModelAnimInfo_MAX_FILENAME = 64;
	constexpr auto DAKGRANNY_MAX_MODEL_INDEX = 2072;
}

namespace DLords::Entity
{
	constexpr auto MAX_MONSTER = 240;
}

struct roster_data_t
{
	int unk1[15];
	int unk2[15];
	int count;
	int unk3[6];
	int unk4[6];
	int unk5[10];
	int unk6[17];
	FILE* pFile;
	int unk8[15];
	int unk9[6];
	int unk10[6];
	int unk11[6];
};
static constexpr auto roster_data_t_size = sizeof(roster_data_t);
static_assert(roster_data_t_size == 416);

struct mouse_param_t
{
	int mouse_x;
	int mouse_y;
	int mouse_accel;
};

struct mb_data_t
{
	int iSpoke;
	int iTeleNum;
	int unk1;
	int unk2;
	int unk3;
};
static_assert(sizeof(mb_data_t) == 20);

struct rnd_enc_t
{
	int a;
	int b;
	int c;
	int d;
};

struct spoke_t
{
	spoke_t(std::string sName, std::initializer_list<std::string> sNamesBOL) :
		sName(std::move(sName)), sNamesBOL(sNamesBOL) {
	}

	std::string sName;
	std::vector<std::string> sNamesBOL;
};

static const inline std::unordered_map<int, spoke_t> gSpokes =
{
	{4, {"Arindale_Dojo", {"DOJO02"}}},
	{5, {"Arindale_Inn", {"INN21"}}},
	{6, {"Arindale_Tower", {"TOWER22"}}},
	{7, {"Arindale_Temple", {"TEMPLE05"}}},
	{8, {"Arindale_Armory", {"WEAPON05"}}},
	{9, {"Arindale_Hall", {"GUILDDUNGEON30"}}},
	{10, {"Arindale_Apothecary", {"APOTHECARY07"}}},
	{11, {"Inside_Vartugg", {"VARTUGGFORTRESS17"}}},
	{12, {"Galdryns_Chamber", {"GALDRYNSCHAMBER18"}}},

	{14, {"Inside_Palace", {"INTERIOR_MANOREND"}}},
	{15, {"Fargrove_Theatre", {"DLTOWNMANORA30INV"}}},
	{16, {"Inside_Palace", {"INTERIOR_MANORINV"}}},

	{34, {"Cam_Test",{"CAMTESTA01"}}},
	{35, {"Unk",{"DUNGC180AT512"}}},

	{36, {"Fargrove_Theatre", {"DLTOWNMANORA30", "INTERIOR_JAIL04"}}},

	{37, {"Inside_Palace", {"DLTOWNMANORB08"}}},
	{38, {"Inside_Crypt", {"CRYPT32"}}},

	{39, {"Moongate_Fargrove", {}}},

	{
		40, 
		{
			"Fargrove_Center", 
			{
				"INTERIOR_INN23", "DLTOWNMAIN72", "INTERIOR_WEAPONSHOP11",
				"INTERIOR_HOUSEA02", "INTERIOR_DOJO06", "INTERIOR_MAGE16",
				"INTERIOR_AGUSTUS08"
			}
		}
	},

	{41, {"Fargrove_Church", {"DLTOWNCHURCH32", "INTERIOR_TEMPLE12", "INTERIOR_APOTHECARY04"}}},

	{42, {"Inside_Palace", {"INTERIOR_MANOR66"}}},

	{43, {"Fargrove_Slums", {"DLTOWNTHIEF11", "INTERIOR_CURIO10", "MANSION20"}}},

	{44, {"Inside_Sewer", {"DLTOWNSEWER129"}}},

	{
		45,
		{
			"Inside_ShadowRuins",
			{
				"DLSHADOWRUINSB97",
				"SHADOWREALMV0_1","SHADOWREALMV0_2","SHADOWREALMV0_3","SHADOWREALMV0_4",
				"SHADOWREALMV0_5","SHADOWREALMV0_6","SHADOWREALMV0_7","SHADOWREALMV0_8",
				"SHADOWREALMV0_9","SHADOWREALMV0_10","SHADOWREALMV0_11","SHADOWREALMV0_12",
			}
		}
	},

	{46, {"Inside_Iryntabl", {"MAZEA17", "SWAMPINTERIOR_SOUTH_ONLY14"}}},
	{47, {"Inside_NagaTemple", {"MOORS79", "NAGA_CHAMBER_FINAL26"}}},
	{48, {"Inside_TombOfSouls", {"TOMB20"}}},
	{49, {"Inside_Ulm", {"OLDKEEP79"}}},
	{50, {"The_End", {"DUNGC193", "KHADSCHAMBER34"}}}
};