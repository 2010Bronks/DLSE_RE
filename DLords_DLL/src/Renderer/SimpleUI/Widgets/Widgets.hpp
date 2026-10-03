#pragma once
#include "imgui_internal.h"

namespace SimpleUI
{
	bool Checkbox(const char* label, bool* v);
	bool Button(const char* label, const ImVec2& size_arg, bool isSelected = false);
}