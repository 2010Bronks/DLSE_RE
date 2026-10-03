#pragma once

static constexpr auto DAKEVENT_QUEUESIZE = 255;

// void sub_45D0A0()
enum EDakEventType : uint32_t
{
	eMeleeAttack = 1,
	
	ePlayEfxFromMonster = 2,
	ePlayEfxFromPlayer = 3,

	eBreakProp = 4,
	eRangeAttack = 5,
	eUnk4 = 6,
	eMagicWeapon = 7,

	eSWTrailStart = 40,
	eSWTrailEnd = 41,

	eBoneEmitEnd = 43,

	ePlaySFX = 50,

	eAttachItem = 60,
};

static const std::string DakEventTypeToStr(EDakEventType num)
{
#define GS_NUM_CASE(name) case EDakEventType::name: return #name;

	switch (num)
	{
		GS_NUM_CASE(eMeleeAttack);
		GS_NUM_CASE(ePlayEfxFromMonster);
		GS_NUM_CASE(ePlayEfxFromPlayer);
		GS_NUM_CASE(eBreakProp);
		GS_NUM_CASE(eRangeAttack);
		GS_NUM_CASE(eUnk4);
		GS_NUM_CASE(eMagicWeapon);
		GS_NUM_CASE(eSWTrailStart);
		GS_NUM_CASE(eSWTrailEnd);
		GS_NUM_CASE(eBoneEmitEnd);
		GS_NUM_CASE(ePlaySFX);
		GS_NUM_CASE(eAttachItem);
	default:
		return std::format("unk[{}]", (int)num);
	}

#undef GS_NUM_CASE
}

struct dak_event_t
{
	int senderId;
	int receiverId;
	int type;
	int itemIdx;
	DWORD time;
	int unk6;
	int attachmentFlags;
	int unk8;
};
static_assert(sizeof(dak_event_t) == 0x20, "Invalid dak_event_t size!");

extern int* g_pEventCount;
extern dak_event_t* g_pEvents;