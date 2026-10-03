#include "precompiled.hpp"

// void InitQEvents()

static int* s_pQEvent_count = nullptr;
static int* s_pQEvent_tail = nullptr;
static qevent_t* s_pQEvents = nullptr;

namespace QEvents
{
	int GetCount()
	{
		return s_pQEvent_count ? *s_pQEvent_count : -1;
	}

	int GetTail()
	{
		return s_pQEvent_tail ? *s_pQEvent_tail : -1;
	}

	qevent_t* GetById(int id)
	{
		if (!s_pQEvents)
			return nullptr;

		if (id < 0 || id >= DLords::Net::MAX_QEVENTS)
			return nullptr;

		int count = GetCount();
		if (count == -1 || id > count)
			return nullptr;

		return &s_pQEvents[id];
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("A3 ?? ?? ?? ?? A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C C3", s_pQEvent_count, 1);
	CREATE_GES("A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C C3", s_pQEvent_tail, 1);
	CREATE_GES("68 ?? ?? ?? ?? A3 ?? ?? ?? ?? A3 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C", s_pQEvents, 1);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);