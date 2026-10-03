#pragma once

enum EClassType : uint8_t
{
	Novice = 0,

	Fighter,
	Mage,
	Adept,
	Rogue,

	// Fighter Tier 2
	Knight,
	Marauder,

	// Mage Tier 2
	Sorcerer,
	Battlemage,

	// Adept Tier 2
	Celestial,
	Paladin,

	// Rougue Tier 2
	Hunter,
	Trickster,

	// East school Tier 2
	Samurai,
	Imperial,
	Shugenja,
	Monk,
	Budoka,

	// Ew, woman Tier 2
	Valkyrie,
	Enchantress,

	// Fighter Tier 3
	Lord,
	Deathlord,

	// Mage Tier 3
	Wizzard,
	Warlock,

	// Adept Tier 3
	Stargazer,
	Crusader,

	// Rouge Tier 3
	Rangelord,
	Cabalist,

	// East school Tier 3
	Warlord,
	Warmonger,
	Kenjasai,
	Dragonlord,
	Ninjalord,

	// Ew, woman Tier 3
	WarWitch,

	// Exclusive Tier 3
	ShadowlordClass,

	// Combo Tier 3?
	Druid,
	Nightblade,
	Witch,
	Bladewitch,
	Necromancer,
	Illusionist,
	Assassin,
	WarAngel,
};

enum ESkillType : int32_t
{
	NoSkill = -1,

	Dagger,
	Sword,
	MaceAndHammer,
	Axe,
	Staff,
	Polearm,
	TwoHanded,

	Dual,

	Throw,
	Bow,

	Cloth,
	Leather,
	Chain,
	Scale,
	Plate,
	Shield,
	Parry,

	Arcane,
	Crystal,
	Nether,
	Rune,
	Alchemy,

	Identify,
	Channel,
	MagicWeapon,
	Scribe,

	Sneak,
	Inspect,
	Picklock,
	DisarmTrap,
	PickPocket,

	Athletics,
	Scout,
	Bargain,
	Repair,
	Bash,

	Backstab,
	LethalStrike,
	Stun,
	BleedingWound,
	Lifesteal,
	Hawkeye,
	Ironwill,
	Spellfire,
	ShadowlordSkill,
	KungFu,
	Terror,
	Phantasm,
	DualTwoHanded,
	DragonFire,
	WhirlWild,
	DivineMend,
	Entangle,
	Seduction,
	TwistedMaster,
	Cripple,

	SkillTypeMax,
};

enum ESkillGroup : int32_t
{
	NoGroup = -1,

	Weaponary,
	Defense,
	Magic,
	Thief,
	General,
	Diabolic
};

static const std::string SkillTypeToStr(ESkillType num)
{
#define SKILL_NUM_CASE(name) case ESkillType::name: return #name;

	switch (num)
	{
		SKILL_NUM_CASE(Dagger);
		SKILL_NUM_CASE(Sword);
		SKILL_NUM_CASE(MaceAndHammer);
		SKILL_NUM_CASE(Axe);
		SKILL_NUM_CASE(Staff);
		SKILL_NUM_CASE(Polearm);
		SKILL_NUM_CASE(TwoHanded);
		SKILL_NUM_CASE(Dual);
		SKILL_NUM_CASE(Throw);
		SKILL_NUM_CASE(Bow);
		SKILL_NUM_CASE(Cloth);
		SKILL_NUM_CASE(Leather);
		SKILL_NUM_CASE(Chain);
		SKILL_NUM_CASE(Scale);
		SKILL_NUM_CASE(Plate);
		SKILL_NUM_CASE(Shield);
		SKILL_NUM_CASE(Parry);
		SKILL_NUM_CASE(Arcane);
		SKILL_NUM_CASE(Crystal);
		SKILL_NUM_CASE(Nether);
		SKILL_NUM_CASE(Rune);
		SKILL_NUM_CASE(Alchemy);
		SKILL_NUM_CASE(Identify);
		SKILL_NUM_CASE(Channel);
		SKILL_NUM_CASE(MagicWeapon);
		SKILL_NUM_CASE(Scribe);
		SKILL_NUM_CASE(Sneak);
		SKILL_NUM_CASE(Inspect);
		SKILL_NUM_CASE(Picklock);
		SKILL_NUM_CASE(DisarmTrap);
		SKILL_NUM_CASE(PickPocket);
		SKILL_NUM_CASE(Athletics);
		SKILL_NUM_CASE(Scout);
		SKILL_NUM_CASE(Bargain);
		SKILL_NUM_CASE(Repair);
		SKILL_NUM_CASE(Bash);
		SKILL_NUM_CASE(Backstab);
		SKILL_NUM_CASE(LethalStrike);
		SKILL_NUM_CASE(Stun);
		SKILL_NUM_CASE(BleedingWound);
		SKILL_NUM_CASE(Lifesteal);
		SKILL_NUM_CASE(Hawkeye);
		SKILL_NUM_CASE(Ironwill);
		SKILL_NUM_CASE(Spellfire);
		SKILL_NUM_CASE(ShadowlordSkill);
		SKILL_NUM_CASE(KungFu);
		SKILL_NUM_CASE(Terror);
		SKILL_NUM_CASE(Phantasm);
		SKILL_NUM_CASE(DualTwoHanded);
		SKILL_NUM_CASE(DragonFire);
		SKILL_NUM_CASE(WhirlWild);
		SKILL_NUM_CASE(DivineMend);
		SKILL_NUM_CASE(Entangle);
		SKILL_NUM_CASE(Seduction);
		SKILL_NUM_CASE(TwistedMaster);
		SKILL_NUM_CASE(Cripple);

	default: return std::format("#{}", (int)num);
	}
#undef SKILL_NUM_CASE
}

static const std::string SkillGroupToStr(ESkillGroup num)
{
#define SKILL_GP_NUM_CASE(name) case ESkillGroup::name: return #name;

	switch (num)
	{
		SKILL_GP_NUM_CASE(Weaponary);
		SKILL_GP_NUM_CASE(Defense);
		SKILL_GP_NUM_CASE(Magic);
		SKILL_GP_NUM_CASE(Thief);
		SKILL_GP_NUM_CASE(General);
		SKILL_GP_NUM_CASE(Diabolic);

	default: return std::format("#{}", (int)num);
	}

#undef SKILL_GP_NUM_CASE
}


#define HIDE_FIELD(field) private: field; public:

struct skill_t
{
	uint32_t level;
	uint32_t xp;
};
static_assert(sizeof(skill_t) == 8);

enum EStatType : uint32_t
{
	Strength,
	Intellect,
	Dexterity,
	Agility,
	Vitality,
	Honor,

	StatTypeMax,
};

/*
	15-0 {"Pro Magic", {15,0}},		// adds resist[id]
	16-1 {"Pro Fire", {16,0}},		// adds resist[id]
	17-4 {"Pro Ice", {17,0}},		// adds resist[id]
	18-3 {"Pro Poison", {18,0}},		// adds resist[id]
	19-2 {"Pro Petrify", {19,0}},	// adds resist[id]
	20-5 {"Pro Gas", {20,0}},		// adds resist[id]
*/

enum EResistType : uint32_t
{
	ProMagic,
	ProFire,
	ProPetrify,
	ProPoison,
	ProIce,
	ProGas,

	ResistTypeMax,
};

struct stat_t
{
	int16_t left;
	int16_t right;
};
static_assert(sizeof(stat_t) == 4);

// "0F B7 14 C5 ?? ?? ?? ?? 66 89 56 43" - array of default stats for all races
struct race_stat_t
{
	stat_t stats[EStatType::StatTypeMax];
};
static_assert(sizeof(race_stat_t) == 24);

struct stat_bar_t
{
	int16_t cur;
	int16_t max;
};
static_assert(sizeof(stat_bar_t) == 4);

enum ERaceType : uint16_t
{
	Human = 0,
	Elf,
	Dwarf,
	Wylvan,
	Urgoth,
	Zaur,
	Thrall,
	Grendol = 7,

	// from strings array
	STR_Magic = 16,
	STR_Fire,
	STR_Petrify,
	STR_Poison,
	STR_Ice,
	STR_Gas,
	STR_Nether,
	STR_Blade,
	STR_Holy,
	STR_Unholy,
	STR_Unpreventable,
	STR_Wind,
	STR_Electric,
	STR_Insect,
	STR_Rune = 30,

	STR_Ok = 32,
	STR_Unconscious,
	STR_Stone,
	STR_Dead,
	STR_Ash = 36,

	STR_IcantDoThatRightNow = 38,
};

#define ENUM_ADD_INV_SLOT_ENTRY(name, id, offset) \
  Page_ ## name ## _ ## offset = id + offset

#define ENUM_ADD_INV_SLOT(name, id)        \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 0),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 1),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 2),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 3),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 4),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 5),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 6),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 7),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 8),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 9),    \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 10),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 11),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 12),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 13),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 14),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 15),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 16),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 17),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 18),   \
  ENUM_ADD_INV_SLOT_ENTRY(name, id, 19)


enum EInvSlotIndex : uint32_t
{
	ENUM_ADD_INV_SLOT(Weapons, 0),
	ENUM_ADD_INV_SLOT(Armor, 20),
	ENUM_ADD_INV_SLOT(Jewerly, 40),
	ENUM_ADD_INV_SLOT(PotionsScrolls, 60),
	ENUM_ADD_INV_SLOT(KeysQuest, 80),
	ENUM_ADD_INV_SLOT(JunkBag, 100),

	OnPlayer_Helmet = 120,
	OnPlayer_Charm = 121,
	OnPlayer_Pauldrons = 122,
	OnPlayer_Armor = 123,
	OnPlayer_Belt = 124,
	OnPlayer_Pants = 125,
	OnPlayer_Boots = 126,
	OnPlayer_Gauntlets = 127,
	OnPlayer_Band = 128,
	OnPlayer_Ring1 = 129,
	OnPlayer_Ring2 = 130,
	OnPlayer_Weapon = 131,
	OnPlayer_Shield = 132,
	OnPlayer_Range = 133,

	Other_0 = 134,
	Other_275 = 409,
};

#undef ENUM_ADD_INV_SLOT

struct dl_player_inventory_t
{
	dl_item_t slot[20];
};

struct dl_on_player_items_t
{
	dl_item_t helmet;
	dl_item_t charm;
	dl_item_t pauldrons;
	dl_item_t armor;
	dl_item_t belt;
	dl_item_t pants;
	dl_item_t boots;
	dl_item_t gauntlets;
	dl_item_t band;
	dl_item_t ring1;
	dl_item_t ring2;
	dl_item_t weapon;
	dl_item_t shield;
	dl_item_t range;
};

struct dl_player_items_t
{
	dl_player_inventory_t weapons;			// [0;19]
	dl_player_inventory_t armor;			// [20;39]
	dl_player_inventory_t jewerly; 			// [40;59]
	dl_player_inventory_t potions_scrolls;	// [60;79]
	dl_player_inventory_t keys_quest;		// [80;99]
	dl_player_inventory_t junk_bag;			// [100;119]

	dl_on_player_items_t on_player;			// [120;133]

	dl_item_t other[275];					// [134;409]

	/*
		[126;143] rune magic
		[206;234] katals
		[271;274] punches
	*/
};

enum EHeraldyId
{
	TheLadyAndTheLion = 0,
	TheMagician = 1,
	TheAstrologer = 2,
	TheAcrobat = 3,
	TheFoolOfFortune = 4,
	TheDragon = 5,
	TheWarrior = 6,
	TheWarder = 7,
	TheHeirophant = 8,
	TheNightWalker = 9,
	TheAlchemist = 10,
	TheMerchang = 11,
	TheTiger = 12,
	TheProtector = 13,
	TheJusticiar = 14,
	TheConjurer = 15,
	TheHealer = 16,
	TheThief = 17,
	TheMaster = 18,
	TheAngel = 19,
	TheDefender = 20,
	TheConquerer = 21,
	TheRaven = 22,
	TheWizzard = 23,
	TheWatcher = 24,
	TheAvenger = 25,
	TheHunter = 26,
	TheJester = 27,
	TheEagle = 28,
	TheGuardian = 29,
	TheSunAndTheMoon = 30,
	TheFourWinds = 31,
	TheSerpent = 32,
	TheVixen = 33,
	TheScribe = 34,
	TheNoble = 35,
	TheKnaveOfStars = 36,
	TheKnaveOfWands = 37,
	TheKnaveOfSwords = 38,
	TheKnaveOfCups = 39,
};

static const std::string HeraldyToStr(EHeraldyId num)
{
#define HERALDY_NUM_CASE(name) case EHeraldyId::name: buf = #name; break;

	std::string buf = "";

	switch (num)
	{
		HERALDY_NUM_CASE(TheLadyAndTheLion);
		HERALDY_NUM_CASE(TheMagician);
		HERALDY_NUM_CASE(TheAstrologer);
		HERALDY_NUM_CASE(TheAcrobat);
		HERALDY_NUM_CASE(TheFoolOfFortune);
		HERALDY_NUM_CASE(TheDragon);
		HERALDY_NUM_CASE(TheWarrior);
		HERALDY_NUM_CASE(TheWarder);
		HERALDY_NUM_CASE(TheHeirophant);
		HERALDY_NUM_CASE(TheNightWalker);
		HERALDY_NUM_CASE(TheAlchemist);
		HERALDY_NUM_CASE(TheMerchang);
		HERALDY_NUM_CASE(TheTiger);
		HERALDY_NUM_CASE(TheProtector);
		HERALDY_NUM_CASE(TheJusticiar);
		HERALDY_NUM_CASE(TheConjurer);
		HERALDY_NUM_CASE(TheHealer);
		HERALDY_NUM_CASE(TheThief);
		HERALDY_NUM_CASE(TheMaster);
		HERALDY_NUM_CASE(TheAngel);
		HERALDY_NUM_CASE(TheDefender);
		HERALDY_NUM_CASE(TheConquerer);
		HERALDY_NUM_CASE(TheRaven);
		HERALDY_NUM_CASE(TheWizzard);
		HERALDY_NUM_CASE(TheWatcher);
		HERALDY_NUM_CASE(TheAvenger);
		HERALDY_NUM_CASE(TheHunter);
		HERALDY_NUM_CASE(TheJester);
		HERALDY_NUM_CASE(TheEagle);
		HERALDY_NUM_CASE(TheGuardian);
		HERALDY_NUM_CASE(TheSunAndTheMoon);
		HERALDY_NUM_CASE(TheFourWinds);
		HERALDY_NUM_CASE(TheSerpent);
		HERALDY_NUM_CASE(TheVixen);
		HERALDY_NUM_CASE(TheScribe);
		HERALDY_NUM_CASE(TheNoble);
		HERALDY_NUM_CASE(TheKnaveOfStars);
		HERALDY_NUM_CASE(TheKnaveOfWands);
		HERALDY_NUM_CASE(TheKnaveOfSwords);
		HERALDY_NUM_CASE(TheKnaveOfCups);

	default: buf = std::format("#{}", (int)num); break;
	}

#undef HERALDY_NUM_CASE

	static const auto format = [](const std::string& input) -> std::string
		{
			if (input.empty())
				return {};

			size_t capitals = 0;
			for (size_t i = 1; i < input.size(); ++i)
			{
				if (std::isupper(static_cast<unsigned char>(input[i])))
					++capitals;
			}

			std::string result;
			result.resize(input.size() + capitals);

			size_t write_pos = 0;
			result[write_pos++] = input[0];

			for (size_t i = 1; i < input.size(); ++i)
			{
				if (std::isupper(static_cast<unsigned char>(input[i])))
				{
					result[write_pos++] = ' ';
				}

				result[write_pos++] = input[i];
			}

			return result;
		};

	return format(buf);
}


#pragma pack(push, 1)
struct dl_player_t
{
	int iHeartBeatSender;
	int unk2;

	char szName[32];

	int16_t isFemale;
	ERaceType iRaceID;
	int16_t unk3;
	EClassType iClassID;

	int16_t unk4;
	int8_t unk5;
	int8_t unk6;
	int16_t statsOverall;
	int16_t unk8;
	int16_t unk9;
	int16_t modelPrefixId;
	int16_t unk11;
	int8_t unk12;
	int8_t unk13;
	int8_t unk14;
	int8_t unk15;
	int16_t unk16;

	stat_t stats[EStatType::StatTypeMax];

	stat_bar_t Health;
	stat_bar_t Mana;

	__int16 iDexterity3;
	__int16 iDexterity4;

	stat_bar_t DodgeParryChance;

	__int16 iArmor;
	__int16 iAttackSpeed16;

	__int32 N00000506;

	int16_t iCritChance;
	__int16 N0000050C;
	int16_t iReputation;

	__int16 N00000510;
	__int16 N00000512;
	__int16 N00000514;
	__int16 N00000516;
	__int16 N00000518;
	__int16 N0000051A;

	int16_t iStatFromItem1;
	int16_t iStatFromItem2;
	int16_t iStatFromItem3;
	int16_t iStatFromItem4;
	int16_t iStatFromItem5;
	int16_t iStatFromItem6;
	int16_t iStatFromItem7;
	int16_t iStatFromItem8;

	__int16 N0000052C;
	__int16 N0000052E;
	__int16 N00000530;
	__int16 N00000532;

	int32_t iAttackSpeed32;

	__int16 iDefense;
	
	stat_t resists[EResistType::ResistTypeMax];

	__int32 iMoney;
	__int32 iExpPoints; 
	__int32 iExpPoints2;  
	__int32 iSkillPoints; 
	__int32 iBonusPoints;  

	int16_t iLevel16; 
	int32_t iLevel32;

	__int32 N00000437;
	__int32 N00000576;
	__int32 N00000578;
	__int32 N0000057A;
	__int32 N0000057C;
	__int32 N0000057E;
	__int32 N00000580;
	__int32 N00000582;
	__int32 N00000584;
	__int32 iKills;
	__int32 iDeaths;
	__int32 iPvpKills;
	__int32 N0000058C;
	__int32 N0000058E;

	int16_t unk999;
	int16_t unk998;

	skill_t skills[ESkillType::SkillTypeMax];

	int heraldy[18];

	int16_t unk17;
	int16_t unk18;
	int16_t unk19;
	int16_t unk20;
	int32_t unk21;
	int32_t unk22;
	int16_t unk23_l;
	int16_t unk23_h;
	int32_t unk24;
	int32_t unk25;
	int32_t unk26;
	int32_t unk27;
	int32_t unk28;
	int32_t unk29;
	float unk30;
	float unk31;
	int16_t unk32;
	int8_t unk33;
	int16_t unk34;
	int16_t unk35;

	HIDE_FIELD(uint8_t dummy3[1290]);

	dl_player_items_t inventory;

	int32_t unk36;
	vector3 unk37[3];

	HIDE_FIELD(uint8_t dummy4[25992]);
};
#pragma pack(pop)

static constexpr auto dl_player_t_size = sizeof(dl_player_t);
static_assert(dl_player_t_size == 0xBABE); // 47806

static constexpr auto dl_player_offset_of_female = offsetof(dl_player_t, isFemale);
static_assert(dl_player_offset_of_female == 40);

static constexpr auto dl_player_offset_of_race = offsetof(dl_player_t, iRaceID);
static_assert(dl_player_offset_of_race == 42);

static constexpr auto dl_player_offset_of_class = offsetof(dl_player_t, iClassID);
static_assert(dl_player_offset_of_class == 46);

static constexpr auto dl_player_offset_of_stats = offsetof(dl_player_t, stats);
static_assert(dl_player_offset_of_stats == 67);

static constexpr auto dl_player_offset_of_health = offsetof(dl_player_t, Health);
static_assert(dl_player_offset_of_health == 91);

static constexpr auto dl_player_offset_of_mana = offsetof(dl_player_t, Mana);
static_assert(dl_player_offset_of_mana == 95);

static constexpr auto dl_player_offset_of_money = offsetof(dl_player_t, iMoney);
static_assert(dl_player_offset_of_money == 187);

static constexpr auto dl_player_offset_of_level16 = offsetof(dl_player_t, iLevel16);
static_assert(dl_player_offset_of_level16 == 207);

static constexpr auto dl_player_offset_of_level32 = offsetof(dl_player_t, iLevel32);
static_assert(dl_player_offset_of_level32 == 209);

static constexpr auto dl_player_offset_of_skills = offsetof(dl_player_t, skills);
static_assert(dl_player_offset_of_skills == 273);

static constexpr auto dl_player_offset_of_inv = offsetof(dl_player_t, inventory);
static_assert(dl_player_offset_of_inv == 2142);

namespace Skills
{
	using skill_list_t = std::vector<ESkillType>;
	using skill_group_t = std::unordered_map<ESkillGroup, skill_list_t>;

	extern const skill_group_t& GetAll();
	extern ESkillGroup GetGroupBySkill(ESkillType skill);
	extern const skill_list_t* GetSkillsByGroup(ESkillGroup group);

	extern void PrintPlayerSkills(dl_player_t* pPly);
}