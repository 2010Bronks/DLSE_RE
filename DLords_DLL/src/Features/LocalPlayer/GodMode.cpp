#include "precompiled.hpp"

bool gbGodMode = false;
bool gbNoAnyDamage = false;
bool gbReflect = false;
int iReflectMul = 1;

bool ApplyDamage(int targetId, int dmg, int attackerId = 0xFFFFFFFF);

using ApplyDamageTo_t = void(__fastcall*)(int targetId, int dmg, unsigned int attackerId, int* a4, int efxId, int argC, int dmgModifier);
ApplyDamageTo_t pfnApplyDamageTo;
static void __fastcall ApplyDamageTo(int targetId, int dmg, unsigned int attackerId, int* a4, int efxId, int argC, int dmgModifier)
{
	if (!pfnApplyDamageTo)
		return;

	if (gbNoAnyDamage)
	{
		pfnApplyDamageTo(targetId, 0, attackerId, a4, efxId, argC, dmgModifier);
		return;
	}

	if (targetId == 0)
	{
		if (gbReflect && attackerId != -1 && attackerId != 0)
		{
			auto pEntity = GetEntityByIndex(attackerId);
			if (pEntity && pEntity->pMonster)
			{
				LOG("[ApplyDamageTo] Reflected [{}]->[{}] amount of damage to {}: {}", dmg, dmg * iReflectMul, pEntity->pMonster->name, attackerId);
			}
			else
			{
				LOG("[ApplyDamageTo] Reflected [{}]->[{}] amount of damage to [{}]", dmg, dmg * iReflectMul, attackerId);
			}

			targetId = attackerId;
			attackerId = 0;
			dmg *= iReflectMul;

		}
		else if (gbGodMode)
		{
			LOG("[ApplyDamageTo] GodMode protects you from [{}] damage", dmg);
			return;
		}
	}

	pfnApplyDamageTo(targetId, dmg, attackerId, a4, efxId, argC, dmgModifier);
}

static int s_nothing = 0;
bool ApplyDamage(int targetId, int dmg, int attackerId)
{
	if (!pfnApplyDamageTo)
		return false;

	pfnApplyDamageTo(targetId, dmg, attackerId, &s_nothing, -1, -1, 0);
	return true;
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ? ? ? ? F6 C3 ? 74 ? 8D 54 24", pfnApplyDamageTo, ApplyDamageTo);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);