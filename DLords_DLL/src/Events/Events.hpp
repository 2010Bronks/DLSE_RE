#pragma once

#include "Net/net_events.hpp"
#include "Net/dak_events.hpp"
#include "Net/qevents.hpp"

using AwardExp_t = void(__fastcall*)(int plyId, int exp);
using PCGold_t = void(__fastcall*)(int plyId, int gold);
using HitMsg_t = void(__fastcall*)(int attackerId, int victimId, int amount, const char* reason);
using FallDam_t = void(__fastcall*)(int id, int dmg, int flags, int a4, vector3* pos);
using ReqLoot_t = void(__fastcall*)(int takerId, int monId, int itemId);
using DropItem_t = void(__fastcall*)(int slotId, vector3* pos, dl_item_t* pItem);

struct mb_event_t
{
	int isValid;
	int unk1;
	int monId;
	int unk2;
	vector3 pos;
	vector3 rot;
	int pad[10];
	int16_t unk_1;
	int16_t unk_2;
	int16_t unk_3;
	int16_t unk_4;
	int16_t unk_5;
	int16_t unk_6;
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
	int unk14;
	int unk15;
	int16_t unk16;
	int16_t unk17;
};