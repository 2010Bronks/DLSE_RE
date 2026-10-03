#include "precompiled.hpp"

namespace Draw
{
	void Text(const ImColor& color, float x, float y, const std::string& str);
	void Text(const ImColor& color, float x, float y, int flags, const std::string& str);
	void Text(const ImColor& color, ImVec2 pos, const std::string& str);
	void Text(const ImColor& color, ImVec2 pos, int flags, const std::string& str);

	void FillRect(float x, float y, float w, float h, const ImColor& color);
	void FillRect(const ImVec2& pos, const ImVec2& size, const ImColor& color);

	void DrawBox(float x, float y, float w, float h, const ImColor& color);
	void DrawBox(const ImVec2& pos, const ImVec2& size, const ImColor& color);
	void DrawOutlineBox(float x, float y, float w, float h, const ImColor& color);
	void DrawOutlineBox(const ImVec2& pos, const ImVec2& size, const ImColor& color);
}

void Draw::Text(const ImColor& color, float x, float y, const std::string& str)
{
	Text(color, x, y, RenderFlags_None, str);
}

void Draw::Text(const ImColor& color, float x, float y, int flags, const std::string& str)
{
	Text(color, ImVec2(x, y), RenderFlags_None, str);
}

void Draw::Text(const ImColor& color, ImVec2 pos, const std::string& str)
{
	Text(color, pos, RenderFlags_None, str);
}

void Draw::Text(const ImColor& color, ImVec2 pos, int flags, const std::string& str)
{
	if (str.empty())
		return;

	auto pDraw = ImGui::GetBackgroundDrawList();
	const auto pFont = ImGui::GetFont();

	pDraw->PushTextureID(ImGui::GetIO().Fonts->TexID);

	if (flags & RenderFlags_CenterX || flags & RenderFlags_CenterY || flags & RenderFlags_LeftX)
	{
		const auto text_size = pFont->CalcTextSizeA(pFont->FontSize, FLT_MAX, 0.0f, str.c_str());
		if (flags & RenderFlags_CenterX)
			pos.x -= text_size.x / 2;
		if (flags & RenderFlags_CenterY)
			pos.y -= text_size.y / 2;
		if (flags & RenderFlags_LeftX)
			pos.x -= text_size.y;
	}

	if (flags & RenderFlags_Outlined)
	{
		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x - 1, pos.y }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)), str.c_str());
		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x, pos.y - 1 }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)), str.c_str());
		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x - 1, pos.y - 1 }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)), str.c_str());
		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x + 1, pos.y - 1 }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)), str.c_str());

		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x + 1, pos.y + 1 }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)), str.c_str());
		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x + 1, pos.y }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)), str.c_str());
		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x, pos.y + 1 }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)), str.c_str());
		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x - 1, pos.y + 1 }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, 1.f)), str.c_str());
	}

	if (flags & RenderFlags_DropShadow && !(flags & RenderFlags_Outlined))
	{
		pDraw->AddText(pFont, pFont->FontSize, ImVec2{ pos.x + 1, pos.y + 1 }, ImGui::GetColorU32(ImVec4(0.f, 0.f, 0.f, (color.Value.w == 1.f) ? 0.75f : color.Value.w)), str.c_str());
	}

	pDraw->AddText(pFont, pFont->FontSize, pos, color, str.c_str());

	pDraw->PopTextureID();
}

void Draw::FillRect(float x, float y, float w, float h, const ImColor& color)
{
	auto pDraw = ImGui::GetBackgroundDrawList();

	pDraw->AddRectFilled(ImVec2(std::round(x), std::round(y)), ImVec2(std::round(x + w), std::round(y + h)), color, 0, 0);
}

void Draw::FillRect(const ImVec2& pos, const ImVec2& size, const ImColor& color)
{
	FillRect(pos.x, pos.y, size.x, size.y, color);
}

void Draw::DrawBox(float x, float y, float w, float h, const ImColor& color)
{
	FillRect(x, y, w, 1, color);
	FillRect(x, y, 1, h, color);
	FillRect(x + w, y, 1, h, color);
	FillRect(x, y + h, w + 1, 1, color);
}

void Draw::DrawBox(const ImVec2& pos, const ImVec2& size, const ImColor& color)
{
	DrawBox(pos.x, pos.y, size.x, size.y, color);
}

void Draw::DrawOutlineBox(float x, float y, float w, float h, const ImColor& color)
{
	DrawBox(x, y, w, h, ImColor(0.f, 0.f, 0.f, color.Value.w));
	DrawBox(x + 1, y + 1, w - 2, h - 2, color);
	DrawBox(x + 2, y + 2, w - 4, h - 4, ImColor(0.f, 0.f, 0.f, color.Value.w));
}

void Draw::DrawOutlineBox(const ImVec2& pos, const ImVec2& size, const ImColor& color)
{
	DrawOutlineBox(pos.x, pos.y, size.x, size.y, color);
}

namespace Draw
{
	struct debug_draw_data_t
	{
		debug_draw_data_t() : clr(Colors::White), str{} { lifetime.reset(); }
		debug_draw_data_t(std::string_view str) : clr(Colors::White), str(str) { lifetime.reset(); }
		debug_draw_data_t(const ImColor& clr, std::string_view str) : clr(clr), str(str) { lifetime.reset(); }

		ImColor clr;
		std::string str;
		ChronoMeter lifetime;

		inline void clear()
		{
			clr = IM_COL32_WHITE;
			str.clear();
		}
	};

	static std::mutex s_dbg_mtx{};
	static debug_draw_data_t s_dbg_data[32];
	static ImVec2 s_dbg_pos = { 20.0f, 120.0f };

	void OnDrawUI(HookChain_OnDrawUI::ICallback* chain, CHookChainArgs_OnDrawUI& args);

	void Debug(size_t pos, const std::string& str);
	void Debug(size_t pos, const ImColor& clr, const std::string& str);
}

void Draw::Debug(size_t pos, const std::string& str)
{
	Debug(pos, IM_COL32_WHITE, str);
}

void Draw::Debug(size_t pos, const ImColor& clr, const std::string& str)
{
	if (pos >= 32)
		return;

	s_dbg_data[pos] = debug_draw_data_t(clr, str);
}

void Draw::OnDrawUI(HookChain_OnDrawUI::ICallback* chain, CHookChainArgs_OnDrawUI& args)
{
	std::scoped_lock lock(s_dbg_mtx);

	const auto pFont = ImGui::GetFont();
	const auto fFontSize = pFont->FontSize + 2.5f;

	ImVec2 pos = {};
	for (int i = 0; i < 32; i++)
	{
		auto& data = s_dbg_data[i];

		if (data.str.empty())
			continue;

		pos = { s_dbg_pos.x, s_dbg_pos.y + i * fFontSize };

		Text(data.clr, pos, RenderFlags_Outlined, data.str);

		if (data.lifetime.has_elapsed(100ms))
			data.clear();
	}

	chain->callNext(args);
}

REGISTER_CALLBACK(OnDrawUI, Draw::OnDrawUI, HC_PRIORITY_LOW);