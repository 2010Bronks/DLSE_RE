#pragma once

#pragma pack(push, 1)
//?
#if 0
enum EObjtype
{
	NOTYPE = 0,
	BSP,
	MISSILE,
	WHEEL,
	PROP,
	ATTACH,
	MONSTER,
	ITEM,
};
#endif

enum EObjtype : uint8_t
{
	NOTYPE = 0,
	MONSTER = 1,
	ATTACH = 2,
	MISSILE = 3,
	PROP = 4,
	ITEM = 5,
	BSP = 6,
	WHEEL = 7,
};

static const std::string ObjTypeToStr(EObjtype num)
{
#define PACKET_NUM_CASE(name) case EObjtype::name: return #name;

	switch (num)
	{
		PACKET_NUM_CASE(NOTYPE);
		PACKET_NUM_CASE(BSP);
		PACKET_NUM_CASE(MISSILE);
		PACKET_NUM_CASE(WHEEL);
		PACKET_NUM_CASE(PROP);
		PACKET_NUM_CASE(ATTACH);
		PACKET_NUM_CASE(MONSTER);
		PACKET_NUM_CASE(ITEM);

		default: return std::format("#{}", (int)num);
	}
#undef PACKET_NUM_CASE
}

struct objid_t
{
	//union
	//{
	//	struct
	//	{
	//		EObjtype type : 8;
	//		int32_t id : 24;
	//	};
	//
	//	int32_t value;
	//};
	EObjtype type;
	int16_t id;

	int8_t pad;
};

struct dl_monster_sound_t
{
	uint8_t pad[184];
};
static constexpr auto dl_monster_sound_t_size = sizeof(dl_monster_sound_t);

struct dl_monster_t
{
	char name[21];
	int8_t data016;
	int16_t mob_type; // 16 types
	int8_t data018;
	int data01C;
	int data020;
	int data024;
	int data028;
	int data02C;
	int data030;
	int data034;
	int data038;
	int data03C;
	int data040;
	int data044;
	int data048;
	int8_t pad1;
	int16_t iProMagic;
	int16_t iProFire;
	int16_t iProPetrify;
	int16_t iProPoison;
	int16_t iProIce;
	int16_t iProGas;
	int data058;
	int data05C;
	int data060;
	int data064;
	int data068;
	int16_t pad2;
	int level;
	int data070;
	int data074;
	int16_t iDex3;
	int16_t iDex4;
	int16_t iDodgeParry_R;
	int16_t iArmor;
	int data080;
	int data084;
	int data088;
	int data08C;
	int data090;
	int data094;
	int data098;
	int data09C;
	__int16 data0A0;
	__int16 treasureTableId;
	__int16 treasureTableId2;
	__int16 data0A6;
	dl_trtable_t* pTreasureTable;
	dl_trtable_t* pTreasureTable2;
	int16_t data0B0;
	int16_t data0B2;
	int data0B4;
	int data0B8;
	int data0BC;
	int data0C0;
	int data0C4;
	int data0C8;
	int data0CC;
	int data0D0;
	int data0D4;
	int data0D8;
	int data0DC;
	int data0E0;
	int data0E4;
	int data0E8;
	int data0EC;
	int data0F0;
	int data0F4;
	int data0F8;
	int data0FC;
	int data100;
	int data104;
	int data108;
	int data10C;
	int data110;
	int data114;
	int data118;
	int data11C;
	int data120;
	int data124;
	int iAttackSpeed32;
	int data12C;
	int data130;
	int data134;
	int data138;
	int data13C;
	int data140;
	int data144;
	__int16 data148;
	__int16 monId;
	__int16 data14C;
	__int16 iMonSndCount;
	dl_monster_sound_t* pMonSnd;
};
static_assert(sizeof(dl_monster_t) == 340); // 0x154

struct dl_object_unk_t
{
	int objId;
	int slotType;
	char pad1[160];
};
static_assert(sizeof(dl_object_unk_t) == 168);

struct dl_prop_t
{
	char name[22];
	int16_t modelNum;
	int16_t modelNum2;
	int16_t unk1;
	int unk2;
	int flags;
	vector3 unk3;
	int16_t unk4;
	int16_t unk5;
	int16_t soundId;
	int16_t unk6;
	int unk7;
	int16_t unk8;
	int16_t unk9;
};
static_assert(sizeof(dl_prop_t) == 0x40); // 64

struct attach_slot_t // sizeof=0xC
{
    int16_t isAttached;
    int16_t pad;
    struct dl_object_t *pObj;
    int attachmentFlags;
};

// size: 332
struct dl_object_t
{
	dl_object_t* pBspObj;
	dl_object_t* pNextObj;
	vector3 pos;
	vector3 rot;
	int32_t unk020;
	int16_t bspXTile;
	int16_t bspZTile;
	int32_t bspObjId;
	int16_t bspLeafId;
	int16_t unk02c;
	m3x3_t rotMat;
	int32_t iFlags;
	int32_t unk058;
	int32_t unk05C;
	int16_t bspLeafId2;
	int16_t unk060;
	vector3 entityPos;
	attach_slot_t* pAttaches[16];
	int32_t unk0B0;
	objid_t objID;
	float fScaledFrametimeSec;
	int16_t  unk0BC;
	bool hasParent;
	__int8  unk0BF;
	dl_object_t* pPrev;
	dl_object_t* pNext;
	int32_t unk0C8;
	int32_t unk0CC;
	int32_t unk0D0;
	int16_t animNum;
	int16_t unk0D4;
	int32_t unk0D8;
	int16_t unk0DC_l;
	int16_t unk0DC_h;
	void* pAnimInfo;
	int32_t unk0E4;
	void* pAnimInfo2;
	int32_t unk0EC;
	vector3 size;
	vector3 unkff;
	int32_t unk108;
	vector3 unkVec3;
	float unk118;
	int32_t unk11C;
	float unk120;
	float unk124;
	float unk128;
	vector3 maxs;
	vector3 mins;
	int32_t unk_y;
	int32_t unk_z;
};
static_assert(sizeof(dl_object_t) == 0x14C); // 332

enum dl_entity_flags
{
	// »грок держит щит подн€тым.
	RAISE_SHIELD = 1 << 31,

	// ѕроиграть анимацию приземлени€ и получить урон.
	HURT_FROM_FALL = 1 << 12,
	// ѕроиграть анимацию урона от врага и получить урон.
	HURT_FROM_ENEMY = 1 << 19,

	// !!! »нформаци€ об уроне хранитс€ в каких-то переменных.

	// »грок стоит на месте, но камера двигаетс€, будто игрок перемещаетс€ как обычно.
	// »грок проигрывает анимации движени€.
	// »грок становитс€ невидимым дл€ врагов.
	GHOST = 1 << 20,
};

typedef unsigned short Bool16;

struct dl_unk2_t
{
	int unk1A4;
	int unk1A8;
	int unk1AC;
	int unk1B0;
	int unk1B4;
	int unk1B8;
	int unk1BC;
	int unk1C0;
	int unk1C4;
	int unk1C8;
	int unk1CC;
	int unk1D0;
	int unk1D4;
	int unk1D8;
	int unk1DC;
	int unk1E0;
	int unk1E4;
	int unk1E8;
	int unk1EC;
	int unk1F0;
	int unk1F4;
	int unk1F8;
	int unk1FC;
	int unk200;
	int unk204;
	int unk208;
	int unk20C;
	int unk210;
	int unk214;
	int unk218;
	int unk21C;
	int unk220;
	int unk224;
	int unk228;
	int unk22C;
	int unk230;
	int unk234;
	int unk238;
	int unk23C;
	int unk240;
	int unk244;
	int unk248;
	int unk24C;
	int unk250;
	int unk254;
	int unk258;
	int unk25C;
	int unk260;
	int unk264;
	int unk268;
	int unk26C;
	int unk270;
	int unk274;
	int unk278;
	int unk27C;
	int unk280;
	int unk284;
	int unk288;
	int unk28C;
	int unk290;
	int unk294;
	int unk298;
	int unk29C;
	int unk2A0;
	int unk2A4;
	int unk2A8;
	int unk2AC;
	int unk2B0;
	int unk2B4;
	int unk2B8;
	int unk2BC;
	int unk2C0;
	int unk2C4;
	int unk2C8;
	int unk2CC;
	int unk2D0;
	int unk2D4;
	int unk2D8;
	int unk2DC;
	int unk2E0;
	int unk2E4;
	int unk2E8;
	int unk2EC;
	int unk2F0;
	int unk2F4;
	int unk2F8;
	int unk2FC;
	int unk300;
	int unk304;
	int unk308;
	int unk30C;
	int unk310;
	int unk314;
	int unk318;
	int unk31C;
	int unk320;
	int unk324;
	int unk328;
	int unk32C;
	int unk330;
	int unk334;
	int unk338;
	int unk33C;
	int unk340;
	int unk344;
	int unk348;
	int unk34C;
	int unk350;
	int unk354;
	int unk358;
	int unk35C;
	int unk360;
	int unk364;
	int unk368;
	int unk36C;
	int unk370;
	int unk374;
	int unk378;
	int unk37C;
	int unk380;
};
static constexpr auto dl_unk2_t_size = sizeof(dl_unk2_t);

struct dl_mount_t
{
	int16_t id;
	int8_t count;
	int8_t bUnk2;
	int8_t isMounted;
	int8_t unk4;
	int16_t entId;
	int16_t entId2;
	int unk6;
	int unk7;
};
static_assert(sizeof(dl_mount_t) == 0x12); // 18

struct dl_npc_code_t
{
	int size;
	int unk2;
};

struct dl_npc_t
{
	char name[24];
	int32_t entity_id;
	int32_t npc_mon_id;
	int32_t field_0x20;
	int32_t field_0x24;
	uint8_t reserved_0x28[58];
	int16_t db_primary_table_id;
	int16_t db_secondary_table_id;
	int16_t db_max_items;
	uint8_t reserved_0x68[2];
	int32_t* p_item_flags;
	dl_item_t* p_items_old;
	uint8_t reserved_0x72[16];
	int16_t base_drop_percent;
	int16_t extra_drop_percent;
	void* p_code_buff;
	void* p_string_buff;
	void* p_rsp_buff;
	int32_t field_0x92;
	uint8_t reserved_0x96[10];
	uint8_t pad1[200];
	int32_t pad1_0x168;
	int32_t pad1_0x16C;
	int32_t pad1_0x170;
	int32_t pad1_0x174;
	uint8_t pad1_0x178;
	uint8_t attackTarget;
	uint8_t pad1_0x17A;
	uint8_t pad1_align_0x17B;
	int16_t treasure_table_id_1;
	int16_t treasure_table_id_2;
	int16_t max_drop_items;
	int16_t field_0x182;
	int32_t* pSellPrice;
	dl_item_t* pItems;
	int32_t pad3_0x18C;
	int32_t pad3_0x190;
	int32_t pad3_0x194;
	int32_t pad3_0x198;
	int16_t sell_price_multiplier;
	int16_t drop_rate_bonus;
	void* pCodeBuf;
	void* pStringBuf;
	void* pRspBuf;
	int32_t pad3_0x1AC;
	int32_t frametime;
	uint8_t pad3_0x1B4[8];
	int32_t pad3_0x1BC;
	int32_t pad3_0x1C0;
	int32_t pad3_0x1C4;
	int32_t pad3_0x1C8;
	int32_t pad3_0x1CC;
	int32_t pad3_0x1D0;
	int32_t pad3_0x1D4;
	int32_t pad3_0x1D8;
	int32_t pad3_0x1DC;
	int32_t pad3_0x1E0;
	int32_t pad3_0x1E4;
	int32_t pad3_0x1E8;
	int32_t pad3_0x1EC;
	int32_t pad3_0x1F0;
	int32_t pad3_0x1F4;
	int32_t pad3_0x1F8;
	int32_t pad3_0x1FC;
	int32_t pad3_0x200;
	int32_t pad3_0x204;
	int32_t pad3_0x208;
	uint8_t pad3_0x20C[96];
	int32_t pad3_0x26C;
	uint8_t pad3_0x270[250];
	int32_t equip_slot_1;
	int32_t equip_slot_2;
	int32_t equip_slot_3;
	int32_t equip_slot_4;
	int32_t equip_slot_5;
	int32_t equip_slot_6;
	int32_t equip_slot_7;
	int32_t equip_slot_8;
	uint8_t pad3_0x38A[2];
	int32_t pad3_0x38C;
	uint8_t pad3_0x390[224];
	int32_t pad3_0x470;
	uint8_t pad3_0x474[340];
	int32_t pad3_0x5C8;
	uint8_t pad3_0x5CC[172];
	int32_t pad3_0x678;
	int32_t pad3_0x67C;
};
static_assert(sizeof(dl_npc_t) == 0x680); // 1664

struct dl_entity_t
{
	dl_monster_t* pMonster; // NOTE: Player doesn't have this field setted
	dl_object_t* pObject;

	Bool16 isMonster;
	int16_t monsterTemplateId;
	int16_t spawnFlags;
	int16_t entityType;

	unsigned int flags;

	int unk014;
	float unk018;
	float unk01C;

	/* Entity position in the world. */
	vector3 pos;
	vector3 rot; // y - yaw, x/z does nothing
	m3x3_t rotMat;

	int unk05C;
	float entityHeight;
	int unk064;
	int unk068;
	int unk06C;
	float entityOffset;
	int unk074;
	int unk078;
	int unk07C;
	int16_t unk080;
	int16_t unk082;
	float unk084;
	float unk088;
	int unk08C;
	int unk090;
	int unk094;
	int unk098;
	int unk09C;
	float unk0A0;
	float unk0A4;
	float unk0A8;
	float unk0AC;
	int unk0B0;
	int unk0B4;
	int unk0B8;
	int16_t unk0BC;
	int16_t unk0BE;
	int unk0C0;
	int unk0C4;
	int unk0C8;
	int unk0CC;
	int unk0D0;
	int unk0D4;
	int16_t unk0D8;
	int16_t unk0DA;
	int16_t unk0DC;
	int16_t unk0DE;
	int unk0E0;
	vector3 unk0E4;
	int unk0F0;
	int unk0F4;
	int unk0F8;
	int unk0FC;
	int unk100;
	int unk104;
	int unk108;
	int unk10C;
	int nearDist;
	int unk114;
	int hasTarget;
	int attackTarget;
	int unk120;
	int unk124;
	int unk128;
	int16_t objId;
	int16_t unk12E;
	int unk130;
	int16_t unk134;
	int16_t unk136;
	int16_t unk138;
	int16_t unk13A;
	int lootMonId2;
	int lootMonId;
	int unk144;
	int unk148;
	int unk14C;
	int unk150;
	const char* szSpellName;
	int unk158;
	int unk15C;
	int unk160;
	int unk164;
	int npc;
	int unk16C;
	int16_t npcMonId;
	int16_t unk172;
	dl_npc_t* pNpcMem;
	int prevInv[11];
	dl_item_t inventory[10];
	dl_item_t inv_loot[6];
	int postInv[36];

	int16_t iStatFromItem1;
	int16_t iStatFromItem2;
	int16_t iStatFromItem4;
	int16_t iStatFromItem3;
	int16_t iStatFromItem5;
	int16_t iStatFromItem6;

	int16_t unk540;
	int16_t iStrength;

	int16_t iIntellect;
	int16_t iDexterity;
	int16_t iAgility;
	int16_t iVitality;
	int16_t iHonor;
	int16_t iProMagic;

	int16_t iProFire;
	int16_t iProPetrify;
	int16_t iProPoison;
	int16_t iProIce;
	int16_t iProGas;
	int16_t Health_Cur;
	int16_t Health_Max;
	int16_t Mana_Cur;
	int16_t Mana_Max;
	int16_t unk560;
	float unk564;
	int16_t unk568;
	int16_t iDexterity4;
	int16_t iDodgeParry_R;
	int16_t iArmor;
	int16_t iDefense;
	int16_t iStrength3;
	int iAttackSpeed32;
	int unk578;
	char str_unk57C[64];
	int unk5BC;
	int unk5C0;
	int unk5C4;
	int unk5C8;
	int unk5CC;
	int unk5D0;
	vector3 velImpulse;
	float velImpulseLen;
	float unk5E4;
	int unk5E8;
	int unk5EC;
	int16_t mountFlags;
	int16_t riderId;;
	int16_t mountId;
	unsigned __int8 unk5F6;
	unsigned __int8 unk5F7;
	unsigned __int8 unk5F8;
	unsigned __int8 unk5F9;
	int16_t unk5FA;
	int16_t unk5FC;
	int16_t unk5FE;
	int unk600;
	int unk604;
	int unk608;
	int unk60C;
	int unk610;
	int unk614;
	vector3 unk618;
	int unk624;
	int unk628;
	int unk62C;
	int unk630;
	int unk634;
	int unk638;
	int unk63C;
	int unk640;
	int unk644;
	int unk648;
	int unk64C;
	int unk650;
	int unk654;
	int unk658;
	int unk65C;
	int unk660;
	int unk664;
	int unk668;
	int unk66C;
	int unk670;
	int unk674;
	int unk678;
	int unk67C;
	int unk680;
	int unk684;
	int16_t unk688;
	int16_t unk68A;
	int unk68C;
	int unk690;
	int unk694;
	int unk698;
	int unk69C;
	int16_t unk6A0_l;
	int16_t unk6A0_h;
	int16_t unk6A4_l;
	int16_t unk6A4_h;
	int unk6A8;
	int unk6AC;
	int someTime1;
	int someTime2;
	int lootCount;
	int loot[10];
};
static_assert(sizeof(dl_entity_t) == 0x6E4); // 1764

struct struct_613
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
	int unk14;
	int unk15;
	int unk16;
	int unk17;
	int unk18;
	int unk19;
	int unk20;

	int unk31;
	int unk32;
	int unk33;
	int unk34;
	int unk35;
	int unk36;
	int unk37;
	int unk38;
	int unk39;
	int unk40;

	int unk41;
	int unk42;
	int unk43;
	int unk44;
};
static_assert(sizeof(struct_613) == 0x88); // 136
#pragma pack(pop)