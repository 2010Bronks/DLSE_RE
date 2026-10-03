#include "precompiled.hpp"

// FindNextEffectId: E8 ?? ?? ?? ?? 8B F0 83 FE FF 75 0D
// gives id of next free slot for effect
// max 32
// ret -1 if no slot

static effect_t** s_pEffects = nullptr; // 32

static effect_t* __fastcall CreateEffect(int effectId, vector3* pStart, vector3* pEnd, float a4, float a5, int a6, __int16 a7)
{
	using namespace Effects;

	if (!pfnCreateEffect)
		return nullptr;

	auto pEffect = pfnCreateEffect(effectId, pStart, pEnd, a4, a5, a6, a7);

	if (bLog && pEffect)
	{
		LOG("[Effects::CreateEffect] Owner[{}] | EffectID[{}] | SpellID[{}]", pEffect->ownerId, effectId, pEffect->spellId);
	}

	return pEffect;
}

namespace Effects
{
	bool bLog = true;
	CreateEffect_t pfnCreateEffect;

	static void Find()
	{
		CREATE_GEH("E8 ?? ?? ?? ?? 89 04 B5 ?? ?? ?? ?? 89 70 28 8B 0C B5", pfnCreateEffect, CreateEffect);

		CREATE_GES("BE ?? ?? ?? ?? B3 08", s_pEffects, 1);
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	Effects::Find();

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);