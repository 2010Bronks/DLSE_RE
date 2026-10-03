#include "precompiled.hpp"
#include "Widgets.hpp"
#include <assert.h>

bool SimpleUI::Checkbox(const char* label, bool* v)
{
	if (!label || *label == '\0')
	{
		assert(!"[CSimpleUI::Checkbox] invalid label");
		return false;
	}

	if (!v)
	{
		assert(!"[CSimpleUI::Checkbox] invalid variable pointer");
		return false;
	}

	bool res = false;
	if (*v)
	{
		auto col = ImGui::GetStyleColorVec4(ImGuiCol_FrameBgActive);
		col.w = 0.8f;

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBgActive));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, col);

		res = ImGui::Checkbox(label, v);

		ImGui::PopStyleColor(2);
	}
	else
	{
		res = ImGui::Checkbox(label, v);
	}

	return res;
}

bool SimpleUI::Button(const char* label, const ImVec2& size_arg, bool isSelected)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();

	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	ImVec2 rect_size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y * 2.0f);
	const ImRect bb(pos, pos + rect_size);

	ImGui::ItemSize(bb, style.FramePadding.y);

	ImDrawList* draw_list = window->DrawList;

	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

	float text_offset = isSelected ? 20.0f : 0.0f;

	float offset = ((bb.Max.y - bb.Min.y) / 2.0f) - (label_size.y / 2.0f);

	if (hovered || isSelected)
		draw_list->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_ButtonHovered, 0.5f));

	draw_list->AddText
	(
		bb.Min + ImVec2(std::floorf(34.0f + text_offset), offset),
		Draw::Colors::White,
		label
	);

	return pressed;
}