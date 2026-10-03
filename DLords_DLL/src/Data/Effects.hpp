#pragma once

namespace Effects
{
	using CreateEffect_t = effect_t*(__fastcall*)(int effectId, vector3* pStart, vector3* pEnd, float a4, float a5, int a6, __int16 a7);

	extern CreateEffect_t pfnCreateEffect;

	extern bool bLog;
}