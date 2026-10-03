#include "precompiled.hpp"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>

CGameRendererStates gRenderStates;

HWND* g_pGameHWND = nullptr;

renderer_data_t* g_pGameRenderer = nullptr;
renderer_states_t* g_pGameRendererStates = nullptr;

D3DMATRIX* g_pProjectionMatrix = nullptr;
D3DMATRIX* g_pViewMatrix = nullptr;

static bool IsImguiInited = false;
static bool IsImguiOpened = false;

static HWND GetGameWindow()
{
	return g_pGameHWND ? *g_pGameHWND : NULL;
}

CSimpleUI gSimpleUI("DLords");

using Before_BeginScene_t = void(*)();
Before_BeginScene_t pfnBefore_BeginScene;
static void Before_BeginScene()
{
	if (!pfnBefore_BeginScene)
		return;

	pfnBefore_BeginScene();
}

using Card_RenderScene_t = void(*)();
Card_RenderScene_t pfnCard_RenderScene;
static void Card_RenderScene()
{
	if (!pfnCard_RenderScene)
		return;

	if (!g_pGameRenderer || !g_pGameRenderer->IsReady() || !g_pGameHWND)
		return pfnCard_RenderScene();

	if (!gSimpleUI.IsInited())
	{
		gSimpleUI.Init(g_pGameRenderer->GetDevice(), GetGameWindow());
		CCallbackMgr::OnMenuInit().Run();
		return pfnCard_RenderScene();
	}

	gSimpleUI.Render();
	pfnCard_RenderScene();
}

WNDPROC gPrevWndProc{};
static LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (gSimpleUI.WindowProc(hwnd, uMsg, wParam, lParam))
		return TRUE;

	//CHookChainArgs_WindowProc args{};
	//args.emplace_back(hwnd);
	//args.emplace_back(uMsg);
	//args.emplace_back(wParam);
	//args.emplace_back(lParam);
	//
	//return CCallbackMgr::WindowProc().callChain(WindowProc_internal, args);

	return CallWindowProc(gPrevWndProc, hwnd, uMsg, wParam, lParam);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("B9 ? ? ? ? E8 ? ? ? ? 85 C0 75 ? B9", g_pGameRenderer, 1);
	//CREATE_GES("89 3D ? ? ? ? E8 ? ? ? ? 83 C4 ? 80 3D", g_pGameHWND, 2);
	CREATE_GES("8B 0D ?? ?? ?? ?? 51 68 ?? ?? ?? ?? 68", g_pGameHWND, 2);
	CREATE_GES("89 1D ?? ?? ?? ?? 89 1D ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? C7 05 ?? ?? ?? ?? ?? ?? ?? ?? 89 3D", g_pGameRendererStates, 2);
	CREATE_GES("68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 44 24 7C", g_pProjectionMatrix, 1);
	CREATE_GES("68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 44 24 7C", g_pViewMatrix, 6);

	if (g_pGameHWND)
	{
		gPrevWndProc = (WNDPROC)SetWindowLongPtrW(*g_pGameHWND, GWLP_WNDPROC, (LONG_PTR)WindowProc);
		if (!gPrevWndProc)
		{
			LOG_ERROR("[gpGameHWND] Could not hook WindowProc!");
		}
	}

	CREATE_GEH("E8 ? ? ? ? 39 35 ? ? ? ? 74 ? 89 35", pfnCard_RenderScene, Card_RenderScene);
	CREATE_GEH("E8 ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 75 0E 8B 46 04", pfnBefore_BeginScene, Before_BeginScene);

	gRenderStates.Init(g_pGameRendererStates);

	chain->callNext(args);
}

static constexpr auto i = 1920 >> 1;

static void Done(HookChain_Done::ICallback* chain, CHookChainArgs_Done& args)
{
	if (g_pGameHWND)
	{
		SetWindowLongPtrW(*g_pGameHWND, GWLP_WNDPROC, (LONG_PTR)gPrevWndProc);
	}
	else
	{
		assert(!"obosr");
	}

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init, HC_PRIORITY_LOW + 1);
REGISTER_CALLBACK(Done, Done);

CGameRendererStates::CGameRendererStates() :
	m_pData(nullptr), m_bInited(false),
	m_internalData{}
{
	
}

void CGameRendererStates::Init(renderer_states_t* pStates)
{
	if (!pStates)
		return;

	if (m_bInited)
	{
		assert(!"[CGameRendererState::Init] Trying to init while already inited.");
		return;
	}

	m_pData = pStates;
	m_bInited = true;

	memcpy_s(&m_internalData, sizeof(renderer_states_t), m_pData, sizeof(renderer_states_t));
}

renderer_states_t& CGameRendererStates::GetData()
{
	return m_internalData;
}

bool CGameRendererStates::SetData()
{
	if (!m_bInited)
		return false;

	if (*m_pData == m_internalData)
		return false;

	memcpy_s(m_pData, sizeof(renderer_states_t), &m_internalData, sizeof(renderer_states_t));
	return true;
}