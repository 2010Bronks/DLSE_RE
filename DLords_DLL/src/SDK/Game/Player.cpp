#include "precompiled.hpp"

namespace Skills
{
	static const skill_group_t s_skills_ordered =
	{
		{ ESkillGroup::Weaponary,
			{
				Dagger, Sword, MaceAndHammer,
				Axe, Staff, Polearm,
				TwoHanded, Throw, Bow,
			}
		},

		{ ESkillGroup::Defense,
			{
				Cloth, Leather, Chain, Scale,
				Plate, Shield, Parry,
			}
		},

		{ ESkillGroup::Magic,
			{
				Arcane, Crystal, Nether,
				Rune, Alchemy, Scribe,
			}
		},

		{ ESkillGroup::Thief,
			{
				Sneak, Inspect, Picklock,
				DisarmTrap, PickPocket,
			}
		},

		{ ESkillGroup::General,
			{
				Athletics, Scout, Bargain,
				Repair, Bash,
			}
		},

		{ ESkillGroup::Diabolic,
			{
				Dual, Identify, Channel, MagicWeapon,
				Backstab, LethalStrike, Stun, BleedingWound,
				Lifesteal, Hawkeye, Ironwill, Spellfire,
				ShadowlordSkill, KungFu, Terror, Phantasm,
				DualTwoHanded, DragonFire, WhirlWild, DivineMend,
				Entangle, Seduction, TwistedMaster, Cripple,
			}
		},
	};

	const skill_group_t& GetAll()
	{
		return s_skills_ordered;
	}

	ESkillGroup GetGroupBySkill(ESkillType skill)
	{
		for (auto&& [group, groupList] : s_skills_ordered)
		{
			auto it = std::find_if(groupList.begin(), groupList.end(),
				[skill](const ESkillType& curSkill)->bool
				{
					return curSkill == skill;
				});

			if (it != groupList.end())
				return group;

		}

		return ESkillGroup::NoGroup;
	}

	const skill_list_t* GetSkillsByGroup(ESkillGroup group)
	{
		for (auto&& [curGroup, groupList] : s_skills_ordered)
		{
			if (curGroup == group)
				return &groupList;
		}

		return nullptr;
	}

	void PrintPlayerSkills(dl_player_t* pPly)
	{
		if (!pPly)
			return;

		LOG("========== All {} Skills ==========", pPly->szName);

		
		for (auto&& [group, skillList] : s_skills_ordered)
		{
			std::string sBuf{};

			for (auto&& skill : skillList)
			{
				size_t idx = static_cast<size_t>(skill);
				if (pPly->skills[idx].level > 0)
				{
					sBuf += std::format("{}[LV{}|XP{}] ", SkillTypeToStr(skill), pPly->skills[idx].level, pPly->skills[idx].xp);
				}
			}

			if (sBuf.empty())
				continue;

			LOG("[{}] {}", SkillGroupToStr(group), sBuf);
		}

		LOG("========== ============= ==========");
	}
}