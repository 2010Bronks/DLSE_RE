#include "precompiled.hpp"
#include "imgui_internal.h"
#include <map>

#define SUI_COLUMNS_STYLE 0

constexpr float BEGIN_WIDTH = 200.f;
constexpr float BEGIN_HEIGHT = 70.0f;

static constexpr auto FONT_NAME = "C:\\Windows\\Fonts\\georgia.ttf";

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static // https://stackoverflow.com/questions/15966642/how-do-you-tell-lshift-apart-from-rshift-in-wm-keydown-events
WPARAM MapLeftRightKeys(WPARAM vk, LPARAM lParam)
{
	WPARAM new_vk;
	UINT scancode = (lParam & 0x00ff0000) >> 16;
	int extended = (lParam & 0x01000000) != 0;

	switch (vk)
	{
	case VK_SHIFT:
		new_vk = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
		break;
	case VK_CONTROL:
		new_vk = extended ? VK_RCONTROL : VK_LCONTROL;
		break;
	case VK_MENU:
		new_vk = extended ? VK_RMENU : VK_LMENU;
		break;
	default:
		// not a key we map from generic to left/right specialized
		//  just return it.
		new_vk = vk;
		break;
	}

	return new_vk;
}

static ImFont* SafeAddFontFromFileTTF(const char* filename, float size_in_pixels)
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF(filename, size_in_pixels);

	assert(font != nullptr && "Failed to load font from file!");
	return font;
}

CSimpleUI::CSimpleUI(std::string_view sName, const ImVec2& size) :
	m_sName(sName), m_bInited(false), m_bOpened(false),
	m_size(size), m_pos{},
	m_pDevice(nullptr), m_hWindow{},
	m_tabs{}, m_pSelectedTab(nullptr)
{

}

void CSimpleUI::Init(IDirect3DDevice9* pDevice, const HWND& hWindow)
{
	if (!pDevice || !hWindow)
		return;

	m_pDevice = pDevice;
	m_hWindow = hWindow;

	ImGui::CreateContext();

	auto& io = ImGui::GetIO();
	io.IniFilename = NULL;

	//ImGui::StyleColorsDark();

	io.Fonts->Fonts[main_medium] = SafeAddFontFromFileTTF(FONT_NAME, 20.0f);
	io.Fonts->Fonts[main_large] = SafeAddFontFromFileTTF(FONT_NAME, 24.0f);
	io.Fonts->Fonts[main_small] = SafeAddFontFromFileTTF(FONT_NAME, 16.0f);
	io.Fonts->Build();

	ImGui_ImplWin32_Init(m_hWindow);
	ImGui_ImplDX9_Init(m_pDevice);
	ImGui_ImplDX9_CreateDeviceObjects();

	PrepareStyle();

	m_bInited = true;
	m_bOpened = true;

	RegisterTab("Info");
	RegisterTab("Player");
	RegisterTab("World");
	RegisterTab("Stats");
	RegisterTab("Resists");
	RegisterTab("Skills");

	if (m_tabs.size())
	{
		m_pSelectedTab = &m_tabs.front();
	}
}

void CSimpleUI::Render()
{
	if (!m_bInited)
		return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (m_bOpened)
	{
		ImGui::GetIO().MouseDrawCursor = true;
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	}
	else
	{
		ImGui::GetIO().MouseDrawCursor = false;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	}

	CCallbackMgr::OnDrawUI().Run();

	auto& style = ImGui::GetStyle();

	if (m_bOpened)
	{
		const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

		ImGui::SetNextWindowSize(m_size);
		const auto bRes = ImGui::Begin(m_sName.c_str(), nullptr, flags);

		if (bRes)
		{
			if (auto pWindow = ImGui::GetCurrentWindow())
			{
				m_pos = pWindow->Pos;
			}

			DrawInternal();
		}

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void CSimpleUI::PrepareStyle()
{
	auto& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(0.0f, 0.0f);
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(128.0f, 128.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = 0;
	style.ChildBorderSize = 2.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 5.0f;
	style.GrabMinSize = 0.0f;
	style.GrabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.ColorButtonPosition = 1;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
	style.DisplayWindowPadding = ImVec2(19.0f, 19.0f);
	style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
	style.MouseCursorScale = 1.0f;
	style.AntiAliasedLines = 1;
	style.AntiAliasedFill = 1;
	style.CurveTessellationTol = 1.25f;
	style.WindowShadowSize = 15.0f;
	style.WindowShadowOffsetDist = 0.0f;
	style.WindowShadowOffsetAngle = 0.0f;
}

void CSimpleUI::DrawHeader()
{
	auto pWindow = ImGui::GetCurrentWindow();
	auto pDraw = pWindow->DrawList;

	// text and tabs
	pDraw->AddRectFilled
	(
		m_pos,
		m_pos + ImVec2(BEGIN_WIDTH, m_size.y),
		ImGui::GetColorU32(ImGuiCol_ChildBg, 0.5f),
		ImGui::GetStyle().WindowRounding,
		ImDrawCornerFlags_TopRight
	);

	ImGui::PushFont(GImGui->IO.Fonts->Fonts[main_large]);
	ImGui::SetCursorPos(ImVec2(std::floorf(34.f), std::floorf(17.f)));
	ImGui::Text("DLords Mod");
	ImGui::SetCursorPosY(std::floorf(BEGIN_HEIGHT));
	ImGui::PopFont();

	// sub tabs
	pDraw->AddRectFilled
	(
		m_pos + ImVec2(BEGIN_WIDTH, 0.0f),
		m_pos + ImVec2(m_size.x, BEGIN_HEIGHT),
		ImGui::GetColorU32(ImGuiCol_ChildBg, 0.5f),
		ImGui::GetStyle().WindowRounding,
		ImDrawCornerFlags_TopRight
	);

	pDraw->AddLine(m_pos + ImVec2(BEGIN_WIDTH, BEGIN_HEIGHT), m_pos + ImVec2(m_size.x, BEGIN_HEIGHT), Draw::Colors::Black, 0.5f);
	pDraw->AddLine(m_pos + ImVec2(BEGIN_WIDTH, 0.0f), m_pos + ImVec2(BEGIN_WIDTH, m_size.y), Draw::Colors::Black, 0.5f);
}

void CSimpleUI::DrawSubTabs()
{

}

void CSimpleUI::DrawTabs()
{
#if SUI_COLUMNS_STYLE
	static bool ensure_column_width = false;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));

	if (!ensure_column_width)
	{
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, 200.0f);

		ensure_column_width = true;
	}
	else
	{
		ImGui::Columns(2, nullptr, false);
	}

	ImGui::PopStyleVar(3);

	for (auto&& cur : m_tabs)
	{
		if (cur.name.empty())
			continue;

		const ImVec2 btn_size = ImVec2(BEGIN_WIDTH, 0.0f);

		const bool isSelected = (cur == m_pSelectedTab);
		if (SimpleUI::Button(cur.name.c_str(), btn_size, isSelected))
		{
			m_pSelectedTab = &cur;
		}
	}

	ImGui::NextColumn();
#else
	bool res = false;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0.0f, 0.0f));

	//float spacing = 20.0f;

	ImVec2 childPos(0.0f, 0.0f);
	ImVec2 childSize(BEGIN_WIDTH, BEGIN_HEIGHT);

	ImGui::PushFont(GImGui->IO.Fonts->Fonts[main_small]);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 8.0f));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetColorU32(ImGuiCol_ChildBg, 0.65f));

	ImGui::SetNextWindowPos(m_pos + childPos);
	res = ImGui::BeginChild(ImGui::GetID("##tabs"), ImVec2(childSize.x, m_size.y), false);

	if (res)
	{
#if SUI_COLUMNS_STYLE == 0
		DrawHeader();
#endif

		for (auto&& cur : m_tabs)
		{
			if (cur.name.empty())
				continue;

			const ImVec2 btn_size = ImVec2(BEGIN_WIDTH, 0.0f);

			const bool isSelected = (cur == m_pSelectedTab);
			if (SimpleUI::Button(cur.name.c_str(), btn_size, isSelected))
			{
				m_pSelectedTab = &cur;
			}
		}
	}

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopFont();
	ImGui::PopStyleVar(3);

	ImGui::EndChild();


#endif
}

void CSimpleUI::DrawContent()
{
	if (!m_pSelectedTab)
	{
#if SUI_COLUMNS_STYLE
		ImGui::Columns(1);
#endif
		return;
	}

	auto pWindow = ImGui::GetCurrentWindow();

	float spacing = 20.0f;

	ImVec2 childPos(BEGIN_WIDTH + spacing, BEGIN_HEIGHT + spacing);
	ImVec2 childSize(m_size.x - BEGIN_WIDTH - spacing * 2.0f, m_size.y - BEGIN_HEIGHT - spacing * 2.0f);

	ImGui::PushFont(GImGui->IO.Fonts->Fonts[main_small]);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 8.0f));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetColorU32(ImGuiCol_ChildBg, 0.65f));

	ImGui::SetNextWindowPos(m_pos + childPos);
	if (ImGui::BeginChild(ImGui::GetID("##content_left"), ImVec2(childSize.x, childSize.y), true))
	{
		m_pSelectedTab->fnDraw();
	}
	ImGui::EndChild();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopFont();

#if SUI_COLUMNS_STYLE
	ImGui::Columns(1);
#endif
}

void CSimpleUI::DrawInternal()
{
	auto& style = ImGui::GetStyle();
	auto displaySize = ImGui::GetIO().DisplaySize;
	auto drawList = ImGui::GetBackgroundDrawList();

#if SUI_COLUMNS_STYLE
	DrawHeader();
#endif

	DrawSubTabs();

	DrawTabs();

	DrawContent();
}

bool CSimpleUI::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!m_bInited)
		return false;

	if (GetFocus() != m_hWindow)
		return false;

	if (uMsg == WM_SYSKEYUP || uMsg == WM_KEYUP)
	{
		auto key = MapLeftRightKeys(wParam, lParam);

		if (key == VK_INSERT || key == VK_END)
			m_bOpened ^= true;
	}

	if (!m_bOpened)
		return false;

	ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
	switch (uMsg)
	{
		// Keyboard
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		// Mouse buttons
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
		// Mouse move + wheel
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
		// System symbols
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSCHAR:
	case WM_SYSDEADCHAR:
		return true;
	}

	return false;
}

void CSimpleUI::RegisterTabCallback(std::string_view name, const fnRenderCallback& cb)
{
	auto tab_it = std::find_if(m_tabs.begin(), m_tabs.end(), [name](const sui_tab_t& curTab)->bool
	{
		return curTab.name == name;
	});

	if (tab_it != m_tabs.end())
	{
		tab_it._Ptr->fnDraw = cb;
	}
	else
	{
		LOG_IMPORTANT("[CSimpleUI::RegisterTabCallback] Trying to register callback for [{}] tab which doesn't exist!", name);
	}
}

void CSimpleUI::RegisterTab(std::string_view name)
{
	m_tabs.push_back(name);
}