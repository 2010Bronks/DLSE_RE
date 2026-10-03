#pragma once

struct spell_data_t
{
	int16_t spellId;
	int16_t unk2;
	int16_t type;
	char name[32];
	int16_t unk5;
	int16_t unk6;
	int16_t unk7;
	int16_t levelNeed;
	int16_t unk9;
	int16_t unk10;
	int16_t unk11;
	int16_t unk12;
	int16_t unk13;
};
static_assert(sizeof(spell_data_t) == 56); // 0x38

namespace Spells
{
	extern size_t GetSpellCount();
	extern spell_data_t* GetSpellList();
	extern spell_data_t* GetSpellData(size_t id);
}

struct missle_t
{
	dl_object_t* pObj;
	m3x3_t mat;
	int16_t pad[2];
	int ownerId;
	int targetId;
	int16_t itemIdx;
	int8_t unk8;
	int8_t unk9;
	int itemId;
	vector3 impulseVec;
	float impulseVecLen;
	int16_t pad3[3];
	int8_t state;
	int8_t unk;
	vector3 pos;
	int unk2;
	int entId2;
	int rot_x;
	int rot_y;
};
static_assert(sizeof(missle_t) == 112);

struct terrain_obj_t
{
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	int unk5;
	int unk6;
	int unk7;
	int unk8;
	int unk9;
	int unk10;
	int unk11;
	int unk12;
	int unk13;

};
static_assert(sizeof(terrain_obj_t) == 52);