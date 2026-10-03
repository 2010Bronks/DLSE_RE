#include "precompiled.hpp"

static keybind_t* s_pKBD_List = nullptr;
static bool* s_pKeysPressed = nullptr;
static command_t* s_pCmds = nullptr;

namespace Binds
{
	keybind_t* GetList()
	{
		if (!s_pKBD_List)
			return nullptr;

		return s_pKBD_List;
	}

	void LogAll()
	{
		if (!s_pKBD_List)
		{
			LOG_IMPORTANT("[Binds::LogAll] List is not ready yet.");
			return;
		}

		LOG("=======BINDS START=======");
		for (auto cur = s_pKBD_List; cur != nullptr; cur = cur->pNext)
		{
			LOG("Key: {:10}\t Cmd: {:16}\t IsPlus: {:4}", GetKeyName(cur->key), cur->command, cur->isPlusCommand);
		}
		LOG("=======BINDS END=======");
	}

	bool *GetKeysPressedList()
	{
		if (!s_pKeysPressed)
			return nullptr;

		return s_pKeysPressed;
	}

	bool IsKeyPressed(size_t keyNum)
	{
		if (!s_pKeysPressed)
			return false;

		if (keyNum >= 256)
			return false;

		return s_pKeysPressed[keyNum];
	}
}

namespace Commands
{
	command_t* GetList()
	{
		if (!s_pCmds)
			return nullptr;

		return s_pCmds;
	}

	void LogAll()
	{
		if (!s_pCmds)
		{
			LOG_WARNING("[Commands::LogAll] CMD list is not ready.");
			return;
		}

		for (auto pCur = s_pCmds; pCur->callback1; pCur++)
		{
			LOG("[Commands::LogAll] cmd -> {}", pCur->name);
		}
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("8B 1D ? ? ? ? 85 DB 0F 84 ? ? ? ? 55", s_pKBD_List, 2);
	CREATE_GES("68 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 83 C4 ? 85 C0 74 ? EB", s_pKeysPressed, 1);
	CREATE_GES("74 49 B8 ?? ?? ?? ?? 8B D0", s_pCmds, 3);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);