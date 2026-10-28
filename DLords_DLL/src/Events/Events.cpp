#include "precompiled.hpp"

namespace Dlords::BSP
{
	static bsp_data_t** pDataArray = nullptr;
	static int* pDataArrayCount = nullptr;

	int GetCount()
	{
		if (!pDataArrayCount)
			return -1;

		return *pDataArrayCount;
	}

	bsp_data_t* GetDataById(size_t id)
	{
		int count = GetCount();
		if (count == -1)
			return nullptr;

		if (id > count)
			return nullptr;

		if (!pDataArray)
			return nullptr;

		auto pData = pDataArray[id];
		if (!pData)
			return nullptr;

		return pData;
	}

	bsp_data_t** Get()
	{
		if (!pDataArray)
			return nullptr;

		return pDataArray;
	}

	static void Find()
	{
		CREATE_GES("B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 8B D7", pDataArray, 1);
		CREATE_GES("8B 15 ?? ?? ?? ?? B9 ?? ?? ?? ?? 89 2D", pDataArrayCount, 2);
	}
}

extern int* g_piSpoke;
extern int* g_piTele;

static std::string GetSpokeStr()
{
	if (!g_piSpoke)
		return "invalid";

	auto spokeData = gSpokes.find(*g_piSpoke);
	if (spokeData == gSpokes.end())
	{
		return std::to_string(*g_piSpoke);
	}
	else
	{
		const auto& [name, list] = spokeData->second;
		return std::format("[{}] {}, cnt[{}]", *g_piSpoke, name, list.size());
	}
}

static std::string GetTeleStr()
{
	if (!g_piTele)
		return "invalid";

	return std::to_string(*g_piTele);
}

using AddImpulseToEntity_T = void(__fastcall*)(int entId, vector3* dir, float power);
AddImpulseToEntity_T pfnAddImpulseToEntity;
static void __fastcall AddImpulseToEntity(int entId, vector3* dir, float power)
{
	if (!pfnAddImpulseToEntity)
		return;

	power = 0.0f;
	pfnAddImpulseToEntity(entId, dir, power);
	//LOG_DBG("[AddImpulseToEntity] entId[{}] dir: {}, {}, {} power[{}]", entId, dir->x, dir->y, dir->z, power);
}

using FillLoot_t = int(__fastcall*)(int entId, dl_trtable_t* pTable, int startSlot, int markAsSecondary);
FillLoot_t pfnFillLoot;
static int __fastcall FillLoot(int entId, dl_trtable_t* pTable, int startSlot, int markAsSecondary)
{
	if (!pfnFillLoot)
		return 0;

	LOG_DBG_IMPORTANT("[FillLoot] entId[{}] startSlot[{}] markAsSecondary[{}]", entId, startSlot, markAsSecondary);

	if (pTable)
	{
		for (int i = 0; i < 10; i++)
		{
			auto& entry = pTable->entries[i];

			if (entry.dropType == EDropType::eDT_Invalid)
				continue;

			LOG_DBG_IMPORTANT("[FillLoot] entry[{}] had dropType[{}] with chance[{}], now will 100% drop!", i, EDropType_Str(entry.dropType), entry.chance);
			entry.chance = 100;
		}
	}

	int res = pfnFillLoot(entId, pTable, startSlot, markAsSecondary);

	return res;
}

static void EveryFrame()
{
	auto pPly = GetLocalPlayer();
	if (!pPly || !pPly->IsValid())
		return;

	auto pEnt = GetEntityByIndex(0);
	if (!pEnt)
		return;

	auto gameState = Game::GetState();
	if (gameState != EGameState::GS_MAX)
	{
		static EGameState prev = EGameState::GS_MAX;

		if (prev == GS_MAX || prev != gameState)
		{
			prev = gameState;
			LOG("Game state changed to: {}", GameStateToStr(gameState));

			//LOG_KBDS();
		}

		vector3 rot = ToSourceAngles(pEnt->rot);

		Draw::Debug(1, Game::GetGameNameVer());
		Draw::Debug(2, GameStateToStr(gameState));
		Draw::Debug(3, std::format("HP: [{}/{}]", pPly->GetHealth(), pPly->GetHealthMax()));
		Draw::Debug(4, std::format("MP: [{}/{}]", pPly->GetMana(), pPly->GetManaMax()));
		Draw::Debug(5, std::format("FPS/MS: [{}/{}]", static_cast<int>(Game::GetFPS()), Game::GetFrametime()));
		Draw::Debug(6, std::format("Race: {}", Game::GetRaceNameById(pPly->GetData()->iRaceID)));
		Draw::Debug(7, std::format("Class: {}", Game::GetClassNameById(pPly->GetData()->iClassID)));
		Draw::Debug(8, std::format("Money: {}", pPly->GetData()->iMoney));
		Draw::Debug(9, std::format("iSpoke: {}", GetSpokeStr()));
		Draw::Debug(10, std::format("Yaw[{}] Norm[{}]", pEnt->rot.y, rot.y));
		Draw::Debug(11, std::format("Vel: [{}] [{}] [{}] [{}]", pEnt->velImpulse.x, pEnt->velImpulse.y, pEnt->velImpulse.z, pEnt->velImpulseLen));
		Draw::Debug(12, std::format("Gravity: [{}]", pEnt->pObject->fScaledFrametimeSec));
		Draw::Debug(12, std::format("Hz next dmg time left: [{}]", pEnt->nextSomethingDamageTime));
		//pEnt->pObject->fScaledFrametimeSec = 0.0f;
	}

	static bool once = true;
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		static bool wasApplied = false;
		if (once)
		{
			once = false;

			if (!wasApplied)
			{
				//EnableMod();
				wasApplied = true;
			}
			else
			{
				//DisableMod();
				wasApplied = false;
			}
		}
	}
	else
	{
		once = true;
	}


	//LOG("Frametime: {}, Fps: {}, Spoke: {}", GetFrametime(), GetFPS());
}

static void OnGameFrame(HookChain_OnGameFrame::ICallback* chain, CHookChainArgs_OnGameFrame& args)
{
	EveryFrame();
	chain->callNext(args);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ?? ?? ?? ?? D9 44 24 1C 8B 8F", pfnAddImpulseToEntity, AddImpulseToEntity);
	CREATE_GEH("E8 ?? ?? ?? ?? 8B F8 EB 1F", pfnFillLoot, FillLoot);

	Dlords::BSP::Find();

	chain->callNext(args);
}

static void OnDllFrame(HookChain_OnDllFrame::ICallback* chain, CHookChainArgs_OnDllFrame& args)
{

	chain->callNext(args);
}

REGISTER_CALLBACK(OnDllFrame, OnDllFrame);
REGISTER_CALLBACK(Init, Init);
REGISTER_CALLBACK(OnGameFrame, OnGameFrame);