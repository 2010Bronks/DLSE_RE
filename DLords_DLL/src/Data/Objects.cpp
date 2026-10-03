#include "precompiled.hpp"

static size_t* s_pObjCount = nullptr;
static dl_object_t* s_pObjList = nullptr;

namespace Objects
{
	bool bLog = true;

	size_t GetObjCount()
	{
		if (!s_pObjCount)
			return -1;

		return *s_pObjCount;
	}

	dl_object_t* GetObjList()
	{
		if (!s_pObjList)
			return nullptr;

		return s_pObjList;
	}

	dl_object_t* GetObjById(size_t objId)
	{
		if (!s_pObjList)
			return nullptr;

		if (objId > GetObjCount())
			return nullptr;

		return &s_pObjList[objId];
	}

	static void Find()
	{
		CREATE_GES("3B 3D ? ? ? ? 0F 8D ? ? ? ? BB", s_pObjCount, 2);
		CREATE_GES("8B 04 B5 ? ? ? ? 85 C0 0F 84 ? ? ? ? F7 80", s_pObjList, 3);
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	Objects::Find();

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);