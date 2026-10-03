#include "precompiled.hpp"

namespace OnPlayTheGameFrame
{
	static void* pAddr = nullptr;

	static int hkFunc()
	{
		CCallbackMgr::OnGameFrame().Run();
		return *reinterpret_cast<int*>(pAddr);
	}

	static void Find()
	{
		auto pattern = gGameModule->Sig().FindSignature("A1 ? ? ? ? 3B C3 8B 15");
		if (!pattern.IsValid())
		{
			LOG_IMPORTANT("[PrepareGameStateFrame] Could not find signature!");
			return;
		}

		void* patch = pattern.Get();
		pAddr = U::Memory::Advance(patch, 1, true);
		U::Memory::CreateMemoryBackup(patch, 5);
		U::Memory::WriteCall(patch, reinterpret_cast<void*>(hkFunc));
	}
}

namespace GameDebug
{
	bool bLog = false;

	static LOG_DEBUG_t pfnLOG_DEBUG;
	static int* s_pInDebug = nullptr;
	static int* s_pShowCoords = nullptr;

	static void LOG_DEBUG(const char* fmt, ...)
	{
		char buf[1024];
		va_list ArgList;

		va_start(ArgList, fmt);
		vsprintf(buf, fmt, ArgList);
		va_end(ArgList);

		pfnLOG_DEBUG("%s", buf);

		if (bLog)
		{
			LOG("{}", buf);
		}
	}

	static void Find()
	{
		CREATE_GEH("E8 ? ? ? ? 83 C4 ? 85 F6 C7 05", pfnLOG_DEBUG, LOG_DEBUG);

		CREATE_GES("83 3D ? ? ? ? ? 53 55 56 57 0F 84", s_pInDebug, 2);
		CREATE_GES("89 1D ? ? ? ? 89 1D ? ? ? ? 74 ? 8B 35", s_pShowCoords, 2);
	}

	int GetDebugState()
	{
		if (!s_pInDebug)
			return -1;

		return *s_pInDebug;
	}

	void SetDebugState(bool state)
	{
		if (!s_pInDebug)
		{
			LOG_IMPORTANT("[GameDebug::SetDebugState] s_pInDebug ptr is not ready!");
			return;
		}

		if (state && GetShowCoords() > 0)
		{
			SetShowCoords(false);
			LOG_IMPORTANT("[GameDebug::SetDebugState] ShowCoords was disabled.");
		}

		*s_pInDebug = static_cast<int>(state);
	}

	int GetShowCoords()
	{
		if (!s_pShowCoords)
			return -1;

		return *s_pShowCoords;
	}

	void SetShowCoords(bool state)
	{
		if (!s_pShowCoords)
		{
			LOG_IMPORTANT("[GameDebug::SetShowCoords] s_pShowCoords ptr is not ready!");
			return;
		}

		if (state && GetDebugState() > 0)
		{
			SetDebugState(false);
			LOG_IMPORTANT("[GameDebug::SetShowCoords] InDebug was disabled.");
		}

		*s_pShowCoords = static_cast<int>(state);
	}
}

namespace OnMonsterSpawn
{
	bool bLog = true;

	static CreateMonsterObject_t pfnCreateMonsterObject;

	static int __fastcall CreateMonsterObject(int monId, vector3* pos, vector3* rot, int attackTarget, int flags, int spawnFlag)
	{
		int res = pfnCreateMonsterObject(monId, pos, rot, attackTarget, flags, spawnFlag);

		if (bLog)
		{
			LOG("[CreateMonsterObject] id[{}] with monId[{}] at pos [{},{},{}] flags[{}] attackTarget[{}] spawnFlag[{}]", res, monId, pos->x, pos->y, pos->z, flags, attackTarget, spawnFlag);
		}

		CHookChainArgs_OnMonsterSpawn args{};
		args.emplace_back(res);
		args.emplace_back(monId);
		args.emplace_back(pos);
		args.emplace_back(rot);
		args.emplace_back(attackTarget);
		args.emplace_back(flags);
		args.emplace_back(spawnFlag);

		CCallbackMgr::OnMonsterSpawn().Run(args);

		return res;
	}

	static void Find()
	{
		CREATE_GEH("E8 ? ? ? ? 8B F8 83 FF ? 0F 84 ? ? ? ? F6 46", pfnCreateMonsterObject, CreateMonsterObject);
	}
}

namespace MonRec
{
	static int* pMonIDCnt = nullptr;
	static int* pMonIDs = nullptr;

	using GetMonsterById_t = dl_entity_t * (__fastcall*)(int monId);

	GetMonsterById_t pfnGetMonsterById;

	static dl_entity_t* __fastcall GetMonsterById(int monId)
	{
		auto res = pfnGetMonsterById(monId);
		//LOG("[GetMonsterById] Mon[{}]", monId);

		return res;
	}

	static void Find()
	{
		CREATE_GES("A1 ? ? ? ? 89 34 85 ? ? ? ? 89 2C 85", pMonIDCnt, 1);
		CREATE_GES("39 0C 85 ? ? ? ? 74 ? 83 C0 ? 3B C2 7C ? 33 C0", pMonIDs, 3);

		CREATE_GEH("E8 ? ? ? ? 33 DB 3B C3 0F 85", pfnGetMonsterById, GetMonsterById);
	}

	size_t GetMonCount()
	{
		if (!pMonIDCnt)
			return 0;

		return static_cast<size_t>(*pMonIDCnt);
	}

	size_t GetMonId(size_t idx)
	{
		if (!pMonIDs)
			return 0;

		size_t monCount = GetMonCount();

		if (monCount == 0)
			return 0;

		if (idx >= monCount)
			return 0;

		return static_cast<size_t>(pMonIDs[idx]);
	}
}

namespace Game
{
	static EGameState* s_pGameState = nullptr;
	static int* s_pFrametime = nullptr;
	static const char** s_pGameNameVer = nullptr;

	static constexpr size_t s_pRacesCnt = 18;
	static constexpr size_t s_pClassesCnt = 48;
	static const char** s_pRaces = nullptr;
	static const char** s_pClasses = nullptr;

	static GetCameraMovement_t pfnGetCameraMovement;

	float GetFrametime()
	{
		if (!s_pFrametime)
			return 0.0f;

		float ms = static_cast<float>(*s_pFrametime);
		if (ms <= 0.0f)
			return 0.0f;

		return ms / 1000.0f;
	}

	float GetFPS()
	{
		auto fFrametime = GetFrametime();
		if (fFrametime <= 0.0f)
			return 0.0f;

		return 1.0f / fFrametime;
	}

	EGameState GetState()
	{
		if (!s_pGameState)
			return EGameState::GS_MAX;

		return *s_pGameState;
	}

	std::string GetRaceNameById(size_t id)
	{
		if (!s_pRaces)
			return std::to_string(id);

		if (id >= s_pRacesCnt)
			return std::to_string(id);

		return s_pRaces[id];
	}

	std::string GetClassNameById(size_t id)
	{
		if (!s_pClasses)
			return std::to_string(id);

		if (id >= s_pClassesCnt)
			return std::to_string(id);

		return s_pClasses[id];
	}

	CCameraMovement* GetCam()
	{
		if (!pfnGetCameraMovement)
			return nullptr;

		return pfnGetCameraMovement();
	}

	std::string GetGameNameVer()
	{
		if (!s_pGameNameVer)
			return "<undefined>";

		return *s_pGameNameVer;
	}

	static void Find()
	{
		CREATE_GES("83 3D ? ? ? ? ? 74 ? B8 ? ? ? ? C3", s_pGameState, 2);
		CREATE_GES("03 2D ? ? ? ? 81 FD", s_pFrametime, 2);
		CREATE_GES("8B 14 8D ? ? ? ? 8B 0D ? ? ? ? 50", s_pRaces, 3);
		CREATE_GES("8B 04 95 ? ? ? ? 0F BF 4E", s_pClasses, 3);
		CREATE_GES("8B 3D ?? ?? ?? ?? 8B C7 8D 50 01 8A 08", s_pGameNameVer, 2);

		auto camera_movement = gGameModule->Sig().FindSignature("E8 ? ? ? ? D9 80 ? ? ? ? 05").Resolve(1, 4);
		pfnGetCameraMovement = reinterpret_cast<GetCameraMovement_t>(camera_movement.Get());
		if (!camera_movement.IsValid())
			LOG_DBG_WARNING("[Game::GetCameraMovement] Could not find signature!");
	}
}

namespace Roster
{
	static roster_data_t* s_pRoster = nullptr;

	roster_data_t* GetData()
	{
		if (!s_pRoster)
			return nullptr;

		return s_pRoster;
	}

	static void Find()
	{
		CREATE_GES("83 3C B5 ? ? ? ? ? A3", s_pRoster, 3);
	}
}

namespace GameDraw
{
	using DrawScreenCenter_t = void(__fastcall*)(const char* str);	
	static DrawScreenCenter_t pfnDrawScreenCenter;
	void __fastcall DrawScreenCenter(const char* str)
	{
		if (!pfnDrawScreenCenter)
			return;

		pfnDrawScreenCenter(str);
	}

	static void Find()
	{
		CREATE_GEH("E8 ?? ?? ?? ?? 8B 1D ?? ?? ?? ?? 89 35 ?? ?? ?? ?? E9 19 F2 FF FF", pfnDrawScreenCenter, DrawScreenCenter);
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	OnPlayTheGameFrame::Find();
	GameDebug::Find();
	OnMonsterSpawn::Find();
	MonRec::Find();
	Game::Find();
	Roster::Find();
	GameDraw::Find();

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);