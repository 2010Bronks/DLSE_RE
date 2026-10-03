#pragma once

namespace Binds
{
	extern keybind_t* GetList();
	extern void LogAll();

	extern bool* GetKeysPressedList();
	extern bool IsKeyPressed(size_t keyNum);
}

namespace Commands
{
	extern command_t* GetList();
	extern void LogAll();
}