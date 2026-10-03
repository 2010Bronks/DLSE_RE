#include "precompiled.hpp"

extern CSimpleUI gSimpleUI;

static constexpr auto s_tab_name = "World";
static bool bCheckbox = true;

extern int* gBsp_Debug;
extern int* gBsp_Bright_Type;

// arrays of gBsp_Bright_Type
extern int* gBsp_Bright_Value;
extern float* gBsp_Bright_Contrast;

extern vector3* gSkybox_Bright;

extern float* gCam_Fov;
extern float* gYonTiles;

extern float* gFog_Start;
extern float* gFog_End;
extern float* gFog_Density;

using pfnSetRenderState = HRESULT(__stdcall*)(float fogStart, float fogEnd, float fogDensity);
static pfnSetRenderState orgSetRenderState;

extern color6b_t* gFog_Col6b;

static int* s_pSpawnedEnemies = nullptr;
static int* s_pNextSpawnTime = nullptr;
static int* s_pTotalSpawnTime = nullptr;

namespace TeleportPlaces
{
	extern std::vector<tele_place_t> wilderness;
	extern std::vector<tele_place_t> interiors;
	extern std::vector<tele_place_t> moonbridges;
}

static void DrawList(std::vector<tele_place_t>& list)
{
	extern bool Teleport(const tele_place_t & place);

	for (auto&& place : list)
	{
		if (ImGui::Button(place.name.c_str()))
		{
			Teleport(place);
		}

	}
}

CFogManager gFogMgr;
static float col[3] = { 0.0f, 0.0f, 0.0f };
static void TabElements()
{
	//if (!gBsp_Bright_Type || !gBsp_Bright_Value || !gBsp_Bright_Contrast ||
	//	!gFog_Color || !gSkybox_Bright || !gCam_Fov ||
	//	!gFog_Start || !gFog_End || !gFog_Density || !gBsp_Debug)
	//{
	//	ImGui::Text("[%s] Not ready!", s_tab_name);
	//	return;
	//}
	//
	//int col[3] = {
	//	gFog_Color->r,
	//	gFog_Color->g,
	//	gFog_Color->b
	//};
	//
	//float bright[3] = {
	//	gSkybox_Bright->x,
	//	gSkybox_Bright->y,
	//	gSkybox_Bright->z
	//};
	//
	//ImGui::SliderInt("Debug", gBsp_Debug, 0, 100);
	//ImGui::SliderInt("Bright type", gBsp_Bright_Type, 0, 50);
	//ImGui::SliderInt("Bright value", &gBsp_Bright_Value[*gBsp_Bright_Type], 0, 1000);
	//ImGui::SliderFloat("Bright contrast", &gBsp_Bright_Contrast[*gBsp_Bright_Type], 0.0f, 1000.0f);
	//
	//if (ImGui::SliderInt3("Fog color", col, 0, 255))
	//{
	//	gFog_Color->r = col[0];
	//	gFog_Color->g = col[1];
	//	gFog_Color->b = col[2];
	//}
	//
	//if (ImGui::SliderFloat3("Skybox Bright", bright, 0.0f, 100.0f))
	//{
	//	gSkybox_Bright->x = bright[0];
	//	gSkybox_Bright->y = bright[1];
	//	gSkybox_Bright->z = bright[2];
	//}
	
	unsigned int days = 0, hours = 0, mins = 0;
	if (WorldTime::Get(days, hours, mins))
	{
		auto time = std::format("Day {}, {}:{}", days, hours, mins);
		ImGui::Text(time.c_str());
	}
	else
	{
		ImGui::Text("Could not get WorldTime.");
	}

	if (SimpleUI::Button("Warp game time", ImVec2(200.0f, 0.0f)))
	{
		WorldTime::Set(999, 13, 37);
	}
	
	ImGui::SliderFloat("CamFOV", gCam_Fov, 0.0f, 5.0f);
	ImGui::InputFloat("gYonTiles", gYonTiles);
	ImGui::InputFloat("FogStart", gFog_Start);
	ImGui::InputFloat("FogEnd", gFog_End);
	ImGui::InputFloat("FogDensity", gFog_Density);
	//
	////ImGui::Text("[%s] Stub text.", s_tab_name);
	////SimpleUI::Checkbox("Checkbox##World", &bCheckbox);

	if (s_pSpawnedEnemies && s_pNextSpawnTime && s_pTotalSpawnTime)
	{
		ImGui::InputInt("SpwnCnt", s_pSpawnedEnemies);
	}
	else
	{
		ImGui::Text("Could not get RndEnc.");
	}

	static const char* szList[] = { "Wilderness","Interiors","Moon bridges" };

	static int list = 0;
	ImGui::Combo("Teleport list", &list, szList, _countof(szList));

	switch (list)
	{
	default:
	case 0:
		DrawList(TeleportPlaces::wilderness);
		break;

	case 1:
		DrawList(TeleportPlaces::interiors);
		break;

	case 2:
		DrawList(TeleportPlaces::moonbridges);
		break;
	}

#if 0
	if (!g_pGameRendererStates)
		return;

	auto& data = gRenderStates.GetData();

	if (SimpleUI::Checkbox("Fog enable", reinterpret_cast<bool*>(&data.fog_enable)))
		gFogMgr.Update(EFogVar::enable);

	if (SimpleUI::Checkbox("Range based", reinterpret_cast<bool*>(&data.range_fog_enable)))
		gFogMgr.Update(EFogVar::range_based);

	if (gBsp_Debug)
	{
		static const char* col_edit_modes[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
		ImGui::Combo("FogDbg", gBsp_Debug, col_edit_modes, IM_ARRAYSIZE(col_edit_modes));
	}

	bool colorChanged = false;
	if (colorChanged = ImGui::ColorPicker3("Fog Color", col))
	{
		gFogMgr.color = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 1.0f));

		if (gFog_Col6b)
		{
			gFog_Col6b->r = static_cast<uint16_t>(col[0] * 255.0f);
			gFog_Col6b->g = static_cast<uint16_t>(col[1] * 255.0f);
			gFog_Col6b->b = static_cast<uint16_t>(col[2] * 255.0f);
		}
	}

	static const char* modes[] = { "D3DFOG_NONE", "D3DFOG_EXP", "D3DFOG_EXP2", "D3DFOG_LINEAR", "D3DFOG_FORCE_DWORD" };
	if (ImGui::Combo("Mode", &gFogMgr.mode, modes, IM_ARRAYSIZE(modes)))
		gFogMgr.Update(EFogVar::mode);

	if (ImGui::Combo("Vertex Mode", &data.fog_vertex_mode, modes, IM_ARRAYSIZE(modes)))
		gFogMgr.Update(EFogVar::mode_vertex);

	if(ImGui::SliderFloat("Start", &data.fog_start, 0.0f, 10000.0f))
		gFogMgr.Update(EFogVar::start);

	if (ImGui::SliderFloat("End", &data.fog_end, 0.0f, 10000.0f))
		gFogMgr.Update(EFogVar::end);

	if (ImGui::SliderFloat("Density", &gFogMgr.density, 0.0f, 10000.0f))
		gFogMgr.Update(EFogVar::density);
#endif
}

static HRESULT __stdcall SetRenderState(float fogStart, float fogEnd, float fogDensity)
{
	if (!orgSetRenderState)
		return 0;

	auto res = orgSetRenderState(fogStart, fogEnd, fogDensity);

	LOG_DBG("[hkSetRenderState::FOG] result[{}] start[{}] end[{}] density[{}]", (int)res, fogStart, fogEnd, fogDensity);

	return res;
}

static void OnGameFrame(HookChain_OnGameFrame::ICallback* chain, CHookChainArgs_OnGameFrame& args)
{
	if (s_pSpawnedEnemies && s_pNextSpawnTime && s_pTotalSpawnTime)
	{
		*s_pSpawnedEnemies = 0;
		*s_pNextSpawnTime = 0;
		*s_pTotalSpawnTime = INT_MAX;
	}

	chain->callNext(args);
}

static void OnMenuInit(HookChain_OnMenuInit::ICallback* chain, CHookChainArgs_OnMenuInit& args)
{
	gSimpleUI.RegisterTabCallback(s_tab_name, TabElements);

	if (g_pGameRenderer)
	{
		if (auto pDevice = g_pGameRenderer->GetDevice())
		{
			gFogMgr.Init(pDevice);

			CREATE_GEH("E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 04 5E 5B", orgSetRenderState, SetRenderState);
		}
	}

	CREATE_GES("8B 35 ?? ?? ?? ?? 75 20", s_pSpawnedEnemies, 2);
	CREATE_GES("89 2D ?? ?? ?? ?? 3B F5 75 22", s_pNextSpawnTime, 2);
	CREATE_GES("01 05 ?? ?? ?? ?? 89 2D", s_pTotalSpawnTime, 2);

	chain->callNext(args);
}

REGISTER_CALLBACK(OnMenuInit, OnMenuInit);
REGISTER_CALLBACK(OnGameFrame, OnGameFrame);