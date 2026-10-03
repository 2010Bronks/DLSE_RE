#include "precompiled.hpp"

namespace TeleportPlaces
{
	std::vector<tele_place_t> wilderness = {
		{"Game Start", {1104878.125f, -4572.452f, 1512414.125f}},
		{"North Fargrove gates", {1002731.688f, -11743.321f, 1309525.500f}},
		{"South Fargrove gates", {1055852.875f, -8434.065f, 1221177.000f}},
		{"Grymlok tower", {1616692.000f, 7943.368f, 1795608.750f}},
		{"Tomb of Souls", {1341144.125f, -2326.495f, 1744729.250f}},
		{"Wyldenfyr", {1817407.875f, -12326.809f, 1896124.000f}},
		{"Shadow crystal", {1959435.375f, -20481.119f, 1938438.500f}},
		{"Altar of Shadows", {1918735.625f, -16229.692f, 1695965.875f}},
		{"Mara's place", {1261941.500f, -6562.362f, 987750.938f}},
		{"Skuldoon center", {1547458.625f, -12361.914f, 688455.125f}},
		{"Staroxia tower", {1430009.000f, -3322.507f, 887023.188f}},
		{"Dungeon of the Moors", {905475.188f, -16501.805f, 758062.375f}},
		{"Worglaw", {990437.562f, -5302.710f, 113196.602f}},
		{"Borderlands of Elven Nation", {515181.781f, -8120.160f, 1016823.188f}},
	};

	std::vector<tele_place_t> interiors = {
		{"Vartuug's Fortress", {201039.906f, -223.535f, 205290.922f}, 11},
		{"The Arcane Emporium", {51043.941f, 32.401f, 262286.625f}, 40},
		{"Arms of Argus", {52887.355f, 32.111f, 103057.328f}, 40},
		{"Davenmor Palace", {204636.094f, 439.952f, 236171.156f}, 40 },
	};

	std::vector<tele_place_t> moonbridges = {
		{"Northland", {1314396.750f, -3041.098f, 1977058.875f}},
		{"Forbidden lands", {1715700.750f, -12206.719f, 614737.812f}},
		{"Arindale", {239713.703f, -7917.316f, 1354441.625f}},
		{"Talendor", {583230.750f, -10712.336f, 2019091.250f}},
		{"Fargrove", {252062.016f, 32.483f, 142865.641f}, 40},
	};
}

int giTeleportButton = VK_RBUTTON;
float gfTeleportDist = 2.0f;

int* g_piTele = nullptr;
int* g_piSpoke = nullptr;

// "E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1"
using TeleportTo_T = void(__fastcall*)(int iSpoke, int iTele, vector3* pos);
static TeleportTo_T pfnTeleportTo = nullptr;
static void __fastcall TeleportTo(int iSpoke, int iTele, vector3* pos)
{
	if (!pfnTeleportTo)
		return;

	pfnTeleportTo(iSpoke, iTele, pos);
	//LOG_DBG("[Deep6::TeleportTo] iSpoke[{}] iTele[{}] Pos: {}, {}, {}", iSpoke, iTele, pos->x, pos->y, pos->z);
}

bool Teleport(const vector3& offset)
{
	if (!pfnTeleportTo)
		return false;

	if (!g_piSpoke || !g_piTele)
		return false;

	auto pMe = GetLocalEntity();
	if (!pMe)
		return false;

	auto pos = pMe->pos + offset;
	TeleportTo(*g_piSpoke, *g_piTele, &pos);

	return true;
}

bool Teleport(float dist)
{
	if (!pfnTeleportTo)
		return false;

	if (!g_piSpoke || !g_piTele)
		return false;

	auto pMe = GetEntityByIndex(0);
	if (!pMe)
		return false;

	vector3 forward, rot;
	rot = ToSourceAngles(pMe->rot);
	GetDirections(&rot, &forward);

	LOG_IMPORTANT("[POS] Spoke[{}] Tele[{}] {:.3f}f, {:.3f}f, {:.3f}f", *g_piSpoke, *g_piTele, pMe->pos.x, pMe->pos.y, pMe->pos.z);

	auto pos = pMe->pos + forward * dist * 1024.f;
	TeleportTo(*g_piSpoke, *g_piTele, &pos);

	return true;
}

bool Teleport(const tele_place_t& place)
{
	if (!pfnTeleportTo)
		return false;

	auto pos = place.pos;
	TeleportTo(place.spoke, place.tele, &pos);
	return true;
}

static void EveryFrame()
{
	static bool once = true;

	if (GetAsyncKeyState(giTeleportButton))
	{
		if (once)
		{
			once = false;

			float mod = 1.0f;
			if (GetAsyncKeyState(VK_LMENU))
				mod = 10.0f;

			if (GetAsyncKeyState(VK_LSHIFT))
				mod *= -1.0f;

			Teleport(gfTeleportDist * mod);
		}
	}
	else
	{
		once = true;
	}
}

static void OnGameFrame(HookChain_OnGameFrame::ICallback* chain, CHookChainArgs_OnGameFrame& args)
{
	EveryFrame();
	chain->callNext(args);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("89 0D ?? ?? ?? ?? D9 46 0C", g_piTele, 2);
	CREATE_GES("83 3D ?? ?? ?? ?? ?? D9 05 ?? ?? ?? ?? 74 06", g_piSpoke, 2);
	CREATE_GEH("E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? A1", pfnTeleportTo, TeleportTo);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);
REGISTER_CALLBACK(OnGameFrame, OnGameFrame);