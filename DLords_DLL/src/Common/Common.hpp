#pragma once

namespace GameDebug
{
	using LOG_DEBUG_t = void(__cdecl*)(const char* fmt, ...);

	extern bool bLog;

	// returns -1 on any error
	extern int GetDebugState();
	extern void SetDebugState(bool state);

	// returns -1 on any error
	extern int GetShowCoords();
	extern void SetShowCoords(bool state);
}

namespace OnMonsterSpawn
{
	using CreateMonsterObject_t = int(__fastcall*)(int monId, vector3* pos, vector3* rot, int attackTarget, int flags, int spawnFlag);

	extern bool bLog;
}

namespace MonRec
{
	extern size_t GetMonCount();
	extern size_t GetMonId(size_t idx);
}

namespace Game
{
	using GetCameraMovement_t = CCameraMovement * (__stdcall*)();

	extern float GetFrametime();
	extern float GetFPS();

	// returns GS_MAX(255) on any error
	extern EGameState GetState();

	extern std::string GetRaceNameById(size_t id);
	extern std::string GetClassNameById(size_t id);
	extern std::string GetGameNameVer();

	extern CCameraMovement* GetCam();
}

namespace Roster
{
	extern roster_data_t* GetData();
}

namespace GameDraw
{
	extern void __fastcall DrawScreenCenter(const char* str);
}