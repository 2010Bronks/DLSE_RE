#pragma once
#include <d3d9.h>
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include "Widgets/Widgets.hpp"

using fnRenderCallback = std::function<void()>;

struct sui_tab_t
{
	sui_tab_t() = delete;
	sui_tab_t(std::string_view name, const fnRenderCallback& cb) :
		name(name), fnDraw(cb)
	{
	}

	sui_tab_t(std::string_view name) :
		name(name), fnDraw{}
	{
	}

	std::string name;
	fnRenderCallback fnDraw;

	__forceinline bool operator==(const sui_tab_t& other)
	{
		return name == other.name;
	}

	__forceinline bool operator==(const sui_tab_t* other)
	{
		if (!other)
			return false;

		return name == other->name;
	}
};

enum fonts_num
{
	main_medium,
	main_large,
	main_small,

	max_fonts_num
};

class CSimpleUI
{
private: // inner variables
	bool m_bInited;
	bool m_bOpened;

	std::string m_sName;

	ImVec2 m_size;
	ImVec2 m_pos;
	sui_tab_t* m_pSelectedTab;

	std::vector<sui_tab_t> m_tabs;

	IDirect3DDevice9* m_pDevice;
	HWND m_hWindow;

private:
	void DrawHeader();
	void DrawSubTabs();
	void DrawTabs();
	void DrawContent();

private:
	void PrepareStyle();
	void DrawInternal();

public:
	CSimpleUI() = delete;
	CSimpleUI(std::string_view sName, const ImVec2& size = ImVec2(800.0f, 600.0f));

	void RegisterTabCallback(std::string_view name, const fnRenderCallback& cb);
	void RegisterTab(std::string_view name);

	const bool IsInited() const { return m_bInited; }
	const bool IsOpened() const { return m_bOpened; }

	void Switch() { m_bOpened ^= true; };

	const ImVec2& GetSize() const { return m_size; }

	void Init(IDirect3DDevice9* pDevice, const HWND& hWindow);
	void Render();
	bool WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};