#pragma once

struct effect_t;

using EfxUpdateCb_t = void(__thiscall*)(effect_t* pEffect);
using EfxRenderCb_t = void(__fastcall*)(effect_t* pEffect, vector3* pPos);

struct efx_obj_t
{
	int id;
	dl_object_t* pObject;
};

struct effect_template_t
{
	int		id;
	int		flags;
	int		subParticleCount;
	vector3	dir;
	int		param1;
	float	speed;
	int		param2;
	float	accel;
	float	gravity;
	float	state0Duration;
	float	state1Duration;
	float	state2Duration;
	int		particleLifetime;
	int		lifetimeVariance;
	int		secondaryBase;
	int		secondaryVariance;
	int		state;
	int		defaultSubId1;
	int		defaultSubId2;
	int		defaultSubId3;
	int		nextState2;
	int		linkedId;
};

struct effect_instance_t
{
	int		id;
	int		flags;
	int		subParticleCount;
	vector3	dir;
	int		param1;
	float	speed;
	float	lifetime;
	float	physicsGravity;
	float	spawnInterval;
	float	state0Duration;
	float	state1Duration;
	float	state2Duration;
	int		particleLifetime;
	int		lifetimeVariance;
	int		secondaryBase;
	int		secondaryVariance;
	int		state;
	int		subCount;
	int		subId1;
	int		subId2;
	int		subId3;
	int		linkedId;
	void* handleId;
	int		activeParticleCount;
	void* particleHead;
	int		pad_6C;
	vector3	localOrigin;
	float	spawnDelay;
	vector3	pos;
};

struct particle_t
{
	void* prev;
	void* next;
	int		state;
	float	timer;
	float	speed;
	int		pad_14;
	vector3	forwardVec;
	float	angle;
	float	velX;
	float	baseAngle;
	float	age;
	int		pad_34;
	int		subFrameCounter;
	int		subEffectIdx;
	int		lifetime;
};

struct effect_def_pair_t
{
	effect_template_t main;
	effect_template_t alt;
};

/*
	Ctor:
		effect_t *__fastcall EFFECT_CREATE(void *a1, vector3 *a2, vector3 *a3, float a4, float a5, int a6, __int16 a7)

	Sig:
		E8 ?? ?? ?? ?? 89 04 B5 ?? ?? ?? ?? 89 78 78 8B 04 B5 ?? ?? ?? ?? 8B 48 7C 83 E9 FF F7 D9 1B C9 23 CD 89 48 7C 8B 14 B5
*/
struct effect_t   // size = 0xC28 = 3112 bytes
{
	// === Transform (0x00-0x1F) ===
	vector3			pos;             // 0x00 — current position
	vector3			rot;             // 0x0C — direction (normalized)
	float			spawnAccum;      // 0x18 — default 0
	float			radiusOrScale;   // 0x1C — from EFFECT_CREATE a4

	// === Identification (0x20-0x27) ===
	int				effectId;        // 0x20
	int				state;           // 0x24 — 0=active, 1=dying

	// === Self-reference (0x28) ===
	int				selfIndex;       // 0x28 — index in g_pEffects

	// === Backup direction (0x2C-0x3B) ===
	vector3			initialDir;      // 0x2C — initial direction

	// === Owner-relative offsets (0x38-0x3F) ===
	float			ownerOffsetY;    // 0x38 — vertical offset from owner
	float			ownerOffsetH;    // 0x3C — horizontal offset from owner

	// === Attachment (0x40-0x47) ===
	int				boneId;          // 0x40
	int				altBoneId;       // 0x44 — alt bone / bone param
	int				time;            // 0x48 — remaining lifetime (ms)

	// === Function pointers (0x4C-0x53) ===
	EfxUpdateCb_t	fnUpdate;  // 0x4C — UPDATE strategy
	EfxRenderCb_t	fnRender;  // 0x50 — RENDER callback

	// === Physics & motion (0x54-0x5F) ===
	float			speed;           // 0x54
	float			pendulumPhase;   // 0x58 — also oscillating alpha for auras
	float			dirLen;          // 0x5C — distance pStart→pEnd

	// === Sub-effects & trail (0x60-0x77) ===
	int				frameCounter;    // 0x60
	int				trailCount;      // 0x64 — also pathHandle for effectId=43
	int				subEffect1;      // 0x68
	int				subEffect2;      // 0x6C
	int				subEffect3;      // 0x70 — often -1
	int				deathState;      // 0x74 — 0=active, 1=dying(sync), ≥2=dead

	// === Entities (0x78-0x83) ===
	int				ownerId;         // 0x78
	int				targetId;        // 0x7C
	int				spellId;         // 0x80

	// === Render (0x84-0x9F) ===
	dl_object_t*	pObject2;    // 0x84
	int				netSyncId;       // 0x88 — MP sync ID (-1 = none)
	float			paramA;          // 0x8C — from a5
	float			float90;         // 0x90 — default 0
	float			computedPosX;    // 0x94
	float			computedPosY;    // 0x98
	float			computedPosZ;    // 0x9C

	// === Rotation (0xA0-0xAB) ===
	vector3			ang;             // 0xA0 — Euler angles

	// === Flags & timers (0xAC-0xBB) ===
	int16_t			flags;           // 0xAC
	int				pad_AE;          // 0xAE
	int				timerAccum33ms;  // 0xB0
	efx_obj_t*		pEfxObj;		 // 0xB4 — visual data
	int				dwordB8;         // 0xB8 — unused in known effects

	// === Scratch buffer (0xBC-0xBFF) ===
	int8_t			scratchBuffer[2880]; // 0xBC — unused in analyzed effects
	//       likely padding/reserved

	// === Trail segment data (0xC00-0xC1F) — for effects 50-55 ===
	float			trailTimeAccum[3];   // 0xC00
	float			trailPosY[3];        // 0xC0C
	int16_t			trailAlpha[4];       // 0xC18

	// === Reserved / audio (0xC20-0xC27) ===
	int				pad_C20;         // 0xC20 — 8-byte aligned, default -1
	int				audChanId;       // 0xC24 — audio channel (-1 = none)
};
static_assert(sizeof(effect_t) == 3112, "Incorrect size of: effect_t");