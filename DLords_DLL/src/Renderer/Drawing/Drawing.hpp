#pragma once

namespace Draw
{
	namespace Colors
	{
		static ImColor Gold = ImColor(255, 220, 0);
		static ImColor Pink = ImColor(255, 89, 217);
		static ImColor Orange = ImColor(255, 178, 51);
		static ImColor Yellow = ImColor(255, 217, 0);
		static ImColor Magenta = ImColor(255, 51, 204);
		static ImColor Red = ImColor(255, 51, 51);
		static ImColor Blue = ImColor(51, 51, 255);
		static ImColor Green = ImColor(51, 255, 51);
		static ImColor SkyBlue = ImColor(10, 150, 200);
		static ImColor DarkerSkyBlue = ImColor(10, 100, 200);
		static ImColor LightGreen = ImColor(200, 255, 128);
		static ImColor DarkGreen = ImColor(70, 138, 53);
		static ImColor Grey = ImColor(127, 127, 127);
		static ImColor LightYellow = ImColor(220, 220, 0);
		static ImColor DeepPink = ImColor(150, 30, 120);
		static ImColor DarkOrange = ImColor(220, 140, 0);
		static ImColor Cyan = ImColor(0, 204, 204);
		static ImColor DarkRed = ImColor(204, 0, 0);
		static ImColor White = ImColor(255, 255, 255);
		static ImColor Black = ImColor(0, 0, 0);
	}
}

namespace Draw
{
	const int RenderFlags_None = 0;
	const int RenderFlags_CenterX = 1 << 0;
	const int RenderFlags_CenterY = 1 << 1;
	const int RenderFlags_LeftX = 1 << 2;
	const int RenderFlags_Outlined = 1 << 3;
	const int RenderFlags_DropShadow = 1 << 4;

	extern void Text(const ImColor& color, float x, float y, const std::string& str);
	extern void Text(const ImColor& color, float x, float y, int flags, const std::string& str);
	extern void Text(const ImColor& color, ImVec2 pos, const std::string& str);
	extern void Text(const ImColor& color, ImVec2 pos, int flags, const std::string& str);

	extern void FillRect(float x, float y, float w, float h, const ImColor& color);
	extern void FillRect(const ImVec2& pos, const ImVec2& size, const ImColor& color);

	extern void DrawBox(float x, float y, float w, float h, const ImColor& color);
	extern void DrawBox(const ImVec2& pos, const ImVec2& size, const ImColor& color);
	extern void DrawOutlineBox(float x, float y, float w, float h, const ImColor& color);
	extern void DrawOutlineBox(const ImVec2& pos, const ImVec2& size, const ImColor& color);
}

namespace Draw
{
	extern void Debug(size_t pos, const std::string& str);
	extern void Debug(size_t pos, const ImColor& clr, const std::string& str);
}