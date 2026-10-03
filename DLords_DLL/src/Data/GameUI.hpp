#pragma once

namespace GameUI
{
	extern bool bLogPageOpen;

	extern bool IsPageOpened(EGuiPages page);
	extern bool OpenPage(EGuiPages page);
	extern bool ClosePage(EGuiPages page);
}

namespace GameUI
{
	extern GraphicResource* GetTexRes(EGuiTexs tex);
	extern const char* GuiTexToStr(EGuiTexs tex);

	// processed texture
	extern ProcessedFrame* GetTex(EGuiTexs tex);
	// raw BMP texture
	extern RawFrame8888* GetTexRaw(EGuiTexs tex);
}