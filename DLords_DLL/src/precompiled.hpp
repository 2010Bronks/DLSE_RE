#pragma once

#pragma region Windows

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <stdint.h>
#include <vector>
#include <list>
#include <memory>
#include <string_view>
#include <string>
#include <functional>
#include <assert.h>
#include <thread>
#include <chrono>
#include <mutex>
#include <format>
#include <stack>
#include <span>

#pragma endregion

#include <Callbacks/callbacks.hpp>
#include <Callbacks/impl/callbacks_impl.hpp>

#include "SDK/dlords_sdk.hpp"

using namespace std::chrono_literals;
#include "Utils/Utils.hpp"
#include "Hooks/Hooks.hpp"
#include "Hooks/Unload.hpp"

#include "Renderer/Renderer.hpp"

#include "Common/Common.hpp"

#include "Data/GameUI.hpp"
#include "Data/GameMap.hpp"
#include "Data/Items.hpp"
#include "Data/KeyBinds.hpp"
#include "Data/Players.hpp"
#include "Data/Entities.hpp"
#include "Data/Objects.hpp"
#include "Data/Magic.hpp"
#include "Data/Effects.hpp"
#include "Data/WorldTime.hpp"

#include "Events/Events.hpp"

extern std::unique_ptr<U::Memory::module_t> gGameModule;

struct tele_place_t
{
	tele_place_t() = default;
	tele_place_t(const std::string& name, const vector3& pos) :
		name(name), pos(pos)
	{
		spoke = 0;
		tele = -1;
	}
	tele_place_t(const std::string& name, const vector3& pos, int spoke) :
		name(name), pos(pos), spoke(spoke)
	{
		tele = -1;
	}

	std::string name;
	vector3 pos;
	int spoke;
	int tele;
};