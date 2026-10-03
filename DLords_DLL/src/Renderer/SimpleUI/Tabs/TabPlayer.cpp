#include "precompiled.hpp"

extern CSimpleUI gSimpleUI;

static constexpr auto s_tab_name = "Player";

extern bool gbGodMode;
extern bool gbReflect;
extern bool gbNoAnyDamage;
extern int iReflectMul;
extern bool gbReduceCD;
extern bool gbInfDurability;

static std::string s_sHeraldys[] =
{
	HeraldyToStr(TheLadyAndTheLion),
	HeraldyToStr(TheMagician),
	HeraldyToStr(TheAstrologer),
	HeraldyToStr(TheAcrobat),
	HeraldyToStr(TheFoolOfFortune),
	HeraldyToStr(TheDragon),
	HeraldyToStr(TheWarrior),
	HeraldyToStr(TheWarder),
	HeraldyToStr(TheHeirophant),
	HeraldyToStr(TheNightWalker),
	HeraldyToStr(TheAlchemist),
	HeraldyToStr(TheMerchang),
	HeraldyToStr(TheTiger),
	HeraldyToStr(TheProtector),
	HeraldyToStr(TheJusticiar),
	HeraldyToStr(TheConjurer),
	HeraldyToStr(TheHealer),
	HeraldyToStr(TheThief),
	HeraldyToStr(TheMaster),
	HeraldyToStr(TheAngel),
	HeraldyToStr(TheDefender),
	HeraldyToStr(TheConquerer),
	HeraldyToStr(TheRaven),
	HeraldyToStr(TheWizzard),
	HeraldyToStr(TheWatcher),
	HeraldyToStr(TheAvenger),
	HeraldyToStr(TheHunter),
	HeraldyToStr(TheJester),
	HeraldyToStr(TheEagle),
	HeraldyToStr(TheGuardian),
	HeraldyToStr(TheSunAndTheMoon),
	HeraldyToStr(TheFourWinds),
	HeraldyToStr(TheSerpent),
	HeraldyToStr(TheVixen),
	HeraldyToStr(TheScribe),
	HeraldyToStr(TheNoble),
	HeraldyToStr(TheKnaveOfStars),
	HeraldyToStr(TheKnaveOfWands),
	HeraldyToStr(TheKnaveOfSwords),
	HeraldyToStr(TheKnaveOfCups),
};

static dl_item_t* GetInvSlot(int page, int slot)
{
	auto pLocal = GetLocalPlayer();
	if (!pLocal)
		return nullptr;

	auto pInv = pLocal->GetInventory(static_cast<EInvSlot>(page));
	if (!pInv)
		return nullptr;

	if (slot < 0 || slot >= 20)
		return nullptr;

	return &pInv->slot[slot];
}

static bool ItemsGetter(void* data, int idx, const char** out_text)
{
	auto pData = reinterpret_cast<dl_item_info_t*>(data);
	if (!pData)
		return false;

	if (idx < 0 || idx > ItemInfo::GetCount())
		return false;

	const auto& item = pData[idx];
	if (strlen(item.name) == 0)
		return false;

	if (out_text)
		*out_text = item.name;

	return true;
}

extern bool gbGravityDisabled;

static int invPage = 0;
static int slot = 0;
static int iCurItem = 0;
static void TabElements()
{
	SimpleUI::Checkbox("GodMode##Player", &gbGodMode);
	SimpleUI::Checkbox("NoAnyDamage##Player", &gbNoAnyDamage);
	SimpleUI::Checkbox("Reflect##Player", &gbReflect);
	ImGui::SliderInt("Reflect Amount##Player", &iReflectMul, 1, 1000);
	SimpleUI::Checkbox("Reduce CDs##Player", &gbReduceCD);
	SimpleUI::Checkbox("Auto repair##Player", &gbInfDurability);
	SimpleUI::Checkbox("Disable Gravity", &gbGravityDisabled);

	static const char* invPages[] = { "Weapons", "Armor", "Jewerly", "Potions & Scrolls", "Keys & Quests", "Junk Bag" };
	ImGui::Combo("Inv page", &invPage, invPages, IM_ARRAYSIZE(invPages));
	ImGui::SliderInt("Inv slot", &slot, 0, 20);

	if (auto pSlot = GetInvSlot(invPage, slot))
	{
		int itemIdx = pSlot->itemIdx;
		if (ImGui::InputInt("Item ID", &itemIdx))
		{
			pSlot->itemIdx = itemIdx;
		}

		int itemFlags = pSlot->flags;
		if (ImGui::InputInt("Item Flags", &itemFlags))
		{
			pSlot->flags = itemFlags;
		}

		int itemCount = pSlot->count;
		if (ImGui::InputInt("Item Count", &itemCount))
		{
			pSlot->count = itemCount;
		}

		int itemDurability = pSlot->durability;
		if (ImGui::InputInt("Item Durability", &itemDurability))
		{
			pSlot->durability = itemDurability;
		}

		auto item = CItemHelper(pSlot);
		auto pRar = item.GetRarity();

		static const char* szRars[] = { "Default", "Trash", "Common", "Green", "Blue", "Violet" };
		static const char* szRar_Common[] = { "Default", "+1", "+2", "+3", "+4", "+5" , "+6" , "+7" , "+8" , "+9" , "+10" };
		static const char* szRar_Green[] = { "Default", "mods(1)", "mods(2)", "mods(3)", "mods(4)", "mods(5)" , "Imbued" , "Empowered" , "Enchanted" , "Exceptional" };
		static const char* szRar_Blue[] = { "Default", "mods(1)", "mods(2)", "Superior(1)", "Superior(2)", "Superior(3)" , "Brilliant" , "Radiant" , "Dazziling" , "Supreme" };
		static const char* szRar_Violet[] = { "Default", "mods(1)", "mods(2)", "mods(3)", "mods(4)", "mods(5)" , "Blessed" , "Devout" , "Divine" , "Elite" };

		if (pRar)
		{
			int rar = pRar->type;
			if (ImGui::Combo("Rarity", &rar, szRars, _countof(szRars)))
			{
				item.SetRarity(EIR_Type(rar));
			}

			int rarMod = pRar->mod;
			switch (EIR_Type(rar))
			{
			default:
			case EIR_Type::EIR_Default:
			case EIR_Type::EIR_Trash:
				// nothing to do here
				break;

			case EIR_Common:
				if (ImGui::Combo("Mod", &rarMod, szRar_Common, _countof(szRar_Common)))
				{
					item.SetRarity(EIR_Type(rar), rarMod);
				}
				break;

			case EIR_Green:
				if (ImGui::Combo("Mod", &rarMod, szRar_Green, _countof(szRar_Green)))
				{
					item.SetRarity(EIR_Type(rar), rarMod);
				}
				break;

			case EIR_Blue:
				if (ImGui::Combo("Mod", &rarMod, szRar_Blue, _countof(szRar_Blue)))
				{
					item.SetRarity(EIR_Type(rar), rarMod);
				}
				break;

			case EIR_Violet:
				if (ImGui::Combo("Mod", &rarMod, szRar_Violet, _countof(szRar_Violet)))
				{
					item.SetRarity(EIR_Type(rar), rarMod);
				}
				break;
			}
		}

		static const char* szMods[21] =
		{
			"None", "Strength", "Intellect", "Dexterity", "Agility", "Vitality" , "Honor" , "Armor" , "Parry" , "Strike", "Critical",
			"Haste", "Damage", "Power", "Influence", "Pro Magic", "Pro Fire" , "Pro Ice" , "Pro Poison" , "Pro Petrify" , "Pro Gas"
		};
		
		for (int i = 0; i < 5; i++)
		{
			int modType = item.GetModType(i);
			int modValue = item.GetModValue(i);

			std::string combo = "Mod type[" + std::to_string(i) + "]";
			std::string input = "Mod value[" + std::to_string(i) + "]";
			if (ImGui::Combo(combo.c_str(), &modType, szMods, _countof(szMods)))
			{
				item.SetMod(i, EIM_Type(modType), modValue);
			}

			if (ImGui::InputInt(input.c_str(), &modValue))
			{
				item.SetMod(i, EIM_Type(modType), modValue);
			}
		}		
	}

	if (auto pItemsList = ItemInfo::GetList())
	{
		auto count = ItemInfo::GetCount();
		if (count > 0)
		{
			if (ImGui::ListBox("All Items", &iCurItem, ItemsGetter, pItemsList, count))
			{
				//if (auto pSlot = GetInvSlot(invPage, slot))
				//{
				//	if (auto pSelItem = ItemInfo::Get(iCurItem))
				//	{
				//		pSlot->itemIdx = iCurItem + 1;
				//		CItemHelper(pSlot).CopyFromInfo(pSelItem);
				//	}
				//}
			}

			if (ImGui::Button("Add item"))
			{
				Item::AddToPlayer(GetLocalPlayer()->GetData(), iCurItem, 1);
			}

			if (ImGui::Button("Spawn item"))
			{
				Item::SpawnInWorld(iCurItem);
			}
		}
	}

	static const char* szHeraldys[] =
	{
		s_sHeraldys[TheLadyAndTheLion].c_str(),
		s_sHeraldys[TheMagician].c_str(),
		s_sHeraldys[TheAstrologer].c_str(),
		s_sHeraldys[TheAcrobat].c_str(),
		s_sHeraldys[TheFoolOfFortune].c_str(),
		s_sHeraldys[TheDragon].c_str(),
		s_sHeraldys[TheWarrior].c_str(),
		s_sHeraldys[TheWarder].c_str(),
		s_sHeraldys[TheHeirophant].c_str(),
		s_sHeraldys[TheNightWalker].c_str(),
		s_sHeraldys[TheAlchemist].c_str(),
		s_sHeraldys[TheMerchang].c_str(),
		s_sHeraldys[TheTiger].c_str(),
		s_sHeraldys[TheProtector].c_str(),
		s_sHeraldys[TheJusticiar].c_str(),
		s_sHeraldys[TheConjurer].c_str(),
		s_sHeraldys[TheHealer].c_str(),
		s_sHeraldys[TheThief].c_str(),
		s_sHeraldys[TheMaster].c_str(),
		s_sHeraldys[TheAngel].c_str(),
		s_sHeraldys[TheDefender].c_str(),
		s_sHeraldys[TheConquerer].c_str(),
		s_sHeraldys[TheRaven].c_str(),
		s_sHeraldys[TheWizzard].c_str(),
		s_sHeraldys[TheWatcher].c_str(),
		s_sHeraldys[TheAvenger].c_str(),
		s_sHeraldys[TheHunter].c_str(),
		s_sHeraldys[TheJester].c_str(),
		s_sHeraldys[TheEagle].c_str(),
		s_sHeraldys[TheGuardian].c_str(),
		s_sHeraldys[TheSunAndTheMoon].c_str(),
		s_sHeraldys[TheFourWinds].c_str(),
		s_sHeraldys[TheSerpent].c_str(),
		s_sHeraldys[TheVixen].c_str(),
		s_sHeraldys[TheScribe].c_str(),
		s_sHeraldys[TheNoble].c_str(),
		s_sHeraldys[TheKnaveOfStars].c_str(),
		s_sHeraldys[TheKnaveOfWands].c_str(),
		s_sHeraldys[TheKnaveOfSwords].c_str(),
		s_sHeraldys[TheKnaveOfCups].c_str(),
	};

	static int heraldy = 0;
	ImGui::Combo("Heraldy", &heraldy, szHeraldys, IM_ARRAYSIZE(szHeraldys));
	ImGui::Text(std::format("Active? {}",
		GetLocalPlayer() ? GetLocalPlayer()->IsHeraldyActive(heraldy) : -1).c_str());

	if (ImGui::Button("Add sel. Heraldy"))
	{
		if (auto pPly = GetLocalPlayer())
		{

			pPly->SetHeraldy(heraldy, true);
		}
	}

	if (ImGui::Button("Rem sel. Heraldy"))
	{
		if (auto pPly = GetLocalPlayer())
		{
			pPly->SetHeraldy(heraldy, false);
		}
	}

	if (ImGui::Button("Log Heraldy"))
	{
		if (auto pPly = GetLocalPlayer())
		{
			for (int i = 0; i < 40; i++)
			{
				LOG("Heraldy[{}] is {}", HeraldyToStr(static_cast<EHeraldyId>(i)), pPly->IsHeraldyActive(i));
			}
		}
	}

	if (ImGui::Button("Add All Heraldy"))
	{
		if (auto pPly = GetLocalPlayer())
		{
			for (int i = 0; i < 40; i++)
			{
				pPly->SetHeraldy(i, true);
			}
		}
	}

	if (ImGui::Button("Remove all Heraldy"))
	{
		if (auto pPly = GetLocalPlayer())
		{
			for (int i = 0; i < 40; i++)
			{
				pPly->SetHeraldy(i, false);
			}
		}
	}
}

static void TabStats()
{
	auto pLocal = GetLocalPlayer();
	if (!pLocal)
		return;

	auto pData = pLocal->GetData();
	if (!pData)
		return;

	static int strength = pData->stats[EStatType::Strength].right;
	if (ImGui::InputInt("Strength", &strength))
	{
		pData->stats[EStatType::Strength].right = strength;
	}

	static int intellect = pData->stats[EStatType::Intellect].right;
	if (ImGui::InputInt("Intellect", &intellect))
	{
		pData->stats[EStatType::Intellect].right = intellect;
	}

	static int dexterity = pData->stats[EStatType::Dexterity].right;
	if (ImGui::InputInt("Dexterity", &dexterity))
	{
		pData->stats[EStatType::Dexterity].right = dexterity;
	}

	static int agility = pData->stats[EStatType::Agility].right;
	if (ImGui::InputInt("Agility", &agility))
	{
		pData->stats[EStatType::Agility].right = agility;
	}

	static int vitality = pData->stats[EStatType::Vitality].right;
	if (ImGui::InputInt("Vitality", &vitality))
	{
		pData->stats[EStatType::Vitality].right = vitality;
	}

	static int honor = pData->stats[EStatType::Honor].right;
	if (ImGui::InputInt("Honor", &honor))
	{
		pData->stats[EStatType::Honor].right = honor;
	}
}

static void TabResists()
{
	auto pLocal = GetLocalPlayer();
	if (!pLocal)
		return;

	auto pData = pLocal->GetData();
	if (!pData)
		return;

	static int proMagic = pData->resists[EResistType::ProMagic].right;
	if (ImGui::InputInt("ProMagic", &proMagic))
	{
		pData->resists[EResistType::ProMagic].right = proMagic;
	}

	static int proFire = pData->resists[EResistType::ProFire].right;
	if (ImGui::InputInt("ProFire", &proFire))
	{
		pData->resists[EResistType::ProFire].right = proFire;
	}

	static int proPetrify = pData->resists[EResistType::ProPetrify].right;
	if (ImGui::InputInt("ProPetrify", &proPetrify))
	{
		pData->resists[EResistType::ProPetrify].right = proPetrify;
	}

	static int proPoison = pData->resists[EResistType::ProPoison].right;
	if (ImGui::InputInt("ProPoison", &proPoison))
	{
		pData->resists[EResistType::ProPoison].right = proPoison;
	}

	static int proIce = pData->resists[EResistType::ProIce].right;
	if (ImGui::InputInt("ProIce", &proIce))
	{
		pData->resists[EResistType::ProIce].right = proIce;
	}

	static int proGas = pData->resists[EResistType::ProGas].right;
	if (ImGui::InputInt("ProGas", &proGas))
	{
		pData->resists[EResistType::ProGas].right = proGas;
	}
}

bool g_bAddLevel = false;
extern bool Teleport(const vector3& offset);
extern bool Teleport(float dist);
static void TabSkills()
{
	auto pLocal = GetLocalPlayer();
	if (!pLocal)
		return;

	auto pData = pLocal->GetData();
	if (!pData)
		return;

	static int group = 0;
	static const char* groups[] = {
		"Weaponary",
		"Defense",
		"Magic",
		"Thief",
		"General",
		"Diabolic",
	};

	static const char* wpns[] = {
		"Dagger", "Sword", "MaceAndHammer",
		"Axe", "Staff", "Polearm",
		"TwoHanded", "Throw", "Bow",
	};

	static const char* defs[] = {
		"Cloth", "Leather", "Chain", "Scale",
		"Plate", "Shield", "Parry",
	};

	static const char* magic[] = {
		"Arcane", "Crystal", "Nether",
		"Rune", "Alchemy", "Scribe",
	};

	static const char* thief[] = {
		"Sneak", "Inspect", "Picklock",
		"DisarmTrap", "PickPocket",
	};

	static const char* general[] = {
		"Athletics", "Scout", "Bargain",
		"Repair", "Bash",
	};

	static const char* diabolic[] = {
		"Dual",
		"Identify",
		"Channel",
		"MagicWeapon",
		"Backstab",
		"LethalStrike",
		"Stun",
		"BleedingWound",
		"Lifesteal",
		"Hawkeye",
		"Ironwill",
		"Spellfire",
		"ShadowlordSkill",
		"KungFu",
		"Terror",
		"Phantasm",
		"DualTwoHanded",
		"DragonFire",
		"WhirlWild",
		"DivineMend",
		"Entangle",
		"Seduction",
		"TwistedMaster",
		"Cripple"
	};

	static int skill = 0;
	if (ImGui::Combo("Group", &group, groups, IM_ARRAYSIZE(groups)))
	{
		skill = 0;
	}

	switch (group)
	{
	default:
	case 0:
		ImGui::Combo("Skill", &skill, wpns, IM_ARRAYSIZE(wpns));
		break;

	case 1:
		ImGui::Combo("Skill", &skill, defs, IM_ARRAYSIZE(defs));
		break;

	case 2:
		ImGui::Combo("Skill", &skill, magic, IM_ARRAYSIZE(magic));
		break;

	case 3:
		ImGui::Combo("Skill", &skill, thief, IM_ARRAYSIZE(thief));
		break;

	case 4:
		ImGui::Combo("Skill", &skill, general, IM_ARRAYSIZE(general));
		break;

	case 5:
		ImGui::Combo("Skill", &skill, diabolic, IM_ARRAYSIZE(diabolic));
		break;
	}

	if (auto pList = Skills::GetSkillsByGroup(static_cast<ESkillGroup>(group)))
	{
		auto id = (*pList)[skill];
		auto& pSkill = pData->skills[id];

		int lvl = pSkill.level;
		int xp = pSkill.xp;
		if (ImGui::InputInt("Level", &lvl))
		{
			pSkill.level = lvl;
		}

		if (ImGui::InputInt("XP", &xp))
		{
			pSkill.xp = xp;
		}
	}

	if (ImGui::Button("Add Level"))
	{
		g_bAddLevel = true;
	}

	static float dist = 1.0f;
	ImGui::InputFloat("TP dist", &dist);
	if (ImGui::Button("Teleport"))
	{
		auto res = Teleport(dist);
		LOG_SUCCESS("Teleport[{}]", res);
	}
}

static void OnMenuInit(HookChain_OnMenuInit::ICallback* chain, CHookChainArgs_OnMenuInit& args)
{
	gSimpleUI.RegisterTabCallback(s_tab_name, TabElements);

	gSimpleUI.RegisterTabCallback("Stats", TabStats);
	gSimpleUI.RegisterTabCallback("Resists", TabResists);
	gSimpleUI.RegisterTabCallback("Skills", TabSkills);

	chain->callNext(args);
}

REGISTER_CALLBACK(OnMenuInit, OnMenuInit);