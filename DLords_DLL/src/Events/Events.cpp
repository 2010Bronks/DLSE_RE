#include "precompiled.hpp"

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

			

			// int __usercall sub_50B5E0@<eax>(int a1@<edx>, int plyId@<ecx>, int a3@<ebp>)

			//LOG("enum EItems ");
			//for (int i = 0; i < ItemInfo::GetCount(); i++)
			//{
			//	auto pItemInfo = ItemInfo::Get(i);
			//	if (!pItemInfo)
			//		continue;
			//
			//	LOG("eI_{} = {},", pItemInfo->name, i);
			//	
			//}
			//LOG(";");

			//auto &pData = pPly->GetData()->inventory;
			//for (int i = 0; i < 275; i++)
			//{
			//	auto& slot = pData.other[i];
			//	if (slot.itemIdx <= 0)
			//		continue;
			//
			//	auto info = ItemInfo::Get(slot.itemIdx - 1);
			//	LOG("slot[{}] [{}] [{}]", i, info->name, slot.count);
			//}
			

			//Skills::PrintPlayerSkills(pPly->GetData());

			//auto pCam = Game::GetCam();
			//if (pCam)
			//{
			//	once = false;
			//}

			//auto iObjCount = Objects::GetObjCount();
			//auto pObjList = Objects::GetObjList();
			//
			//if (iObjCount > 0 && pObjList)
			//{
			//	for (int i = 0; i <= iObjCount; i++)
			//	{
			//		auto& curObj = pObjList[i];
			//		LOG("Obj[{}]: type[{}] itemId[{}]", i, ObjTypeToStr(curObj.objID.type), static_cast<int>(curObj.objID.id));
			//	}
			//}
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

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);
REGISTER_CALLBACK(OnGameFrame, OnGameFrame);