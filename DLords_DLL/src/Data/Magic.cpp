#include "precompiled.hpp"

namespace Spells
{
	static int* s_piSpellCount = nullptr;
	static spell_data_t* s_pSpellList = nullptr;

	// InitSpells()
	static void Find()
	{
		CREATE_GES("B9 ?? ?? ?? ?? 8D 49 00 0F BF 41 02", s_pSpellList, 1);
		CREATE_GES("81 F9 ?? ?? ?? ?? 66 89 14 45", s_piSpellCount, 2);
	}

	size_t GetSpellCount()
	{
		if (!s_piSpellCount)
			return 145;

		if (*s_piSpellCount == 0)
			return 145;

		return *s_piSpellCount;
	}

	spell_data_t* GetSpellList()
	{
		if (!s_pSpellList)
			return nullptr;

		return s_pSpellList;
	}

	spell_data_t* GetSpellData(size_t id)
	{
		if (!s_pSpellList)
			return nullptr;

		auto count = GetSpellCount();
		if (id > count)
			return nullptr;

		return &s_pSpellList[id];
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	Spells::Find();

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);