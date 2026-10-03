#pragma once

#include "callbacks.hpp"

#define DEFINE_EVENT_ARGS_CTOR(EventName)                             \
    template <typename... Args>                                       \
    CHookChainArgs_##EventName(Args&&... args)                        \
    {                                                                 \
        (this->push_back(std::forward<Args>(args)), ...);             \
    }

class CHookChainArgs_Init : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(Init);
};

class CHookChainArgs_Done : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(Done);
};

class CHookChainArgs_OnMenuBuild : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(OnMenuBuild);
};

class CHookChainArgs_OnMenuInit : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(OnMenuInit);
};

class CHookChainArgs_OnDrawUI : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(OnDrawUI);
};

class CHookChainArgs_OnDllFrame : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(OnDllFrame);
};

class CHookChainArgs_OnGameFrame : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(OnGameFrame);
};

class CHookChainArgs_OnMonsterSpawn : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(OnMonsterSpawn);

	// pfnCreateMonsterObject(monId, pos, rot, attackTarget, flags, spawnFlag);
	enum Args
	{
		A_Id = 0,
		A_MonId = 1,
		A_Pos = 2,
		A_Rot = 3,
		A_AtackTarget = 4,
		A_Flags = 5,
		A_SpawnFlags = 6,
	};
};

class CHookChainArgs_OnGuiPageChange : public CHookChainArgs
{
public:
	DEFINE_EVENT_ARGS_CTOR(OnGuiPageChange);

	// vector3* pos, vector3* a3, int a4, int flags, int a6
	enum Args
	{
		A_PageId = 0,   // EGuiPages
		A_State = 1,      // bool
	};
};