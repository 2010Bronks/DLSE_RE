#pragma once

struct effect_t;

using EfxCallback1_t = void(__thiscall*)(effect_t* pEffect);
using EfxCallback2_t = void(__fastcall*)(effect_t* pEffect, vector3* pPos);

struct efx_obj_t
{
	int id;
	dl_object_t* pObject;
};

/*
	Ctor:
		effect_t *__fastcall EFFECT_CREATE(void *a1, vector3 *a2, vector3 *a3, float a4, float a5, int a6, __int16 a7)

	Sig:
		E8 ?? ?? ?? ?? 89 04 B5 ?? ?? ?? ?? 89 78 78 8B 04 B5 ?? ?? ?? ?? 8B 48 7C 83 E9 FF F7 D9 1B C9 23 CD 89 48 7C 8B 14 B5

	
*/
struct effect_t
{
	vector3 pos;
	vector3 rot;
	float float18;
	float float1C;
	int32_t effectId;
	int32_t dword24;
	int8_t gap28[4];
	vector3 rot2;
	float float38;
	float float3C;
	int32_t boneId;
	int32_t dword44;
	int32_t time;
	EfxCallback1_t fnCallback1;
	EfxCallback2_t fnCallback2;
	float speed;
	float float58;
	float dirLen;
	int32_t dword60;
	int32_t dword64;
	int32_t dword68;
	int32_t dword6C;
	int32_t dword70;
	int32_t dword74;
	int32_t ownerId;
	int32_t targetId;
	int spellId;
	dl_object_t* pObject2;
	int32_t dword88;
	float float8C;
	float float90;
	float float94;
	float float98;
	float float9C;
	vector3 ang;
	int16_t flags;
	int32_t dwordB0;
	efx_obj_t* pEfxObj;
	int32_t dwordB8;
	int8_t gapBC[2884];
	float floatC00;
	int8_t gapC04[8];
	float floatC0C;
	int8_t gapC10[8];
	int16_t wordC18;
	__declspec(align(8)) int32_t dwordC20;
	int32_t audChanId;
};
static_assert(sizeof(effect_t) == 3112, "Incorrect size of: effect_t");