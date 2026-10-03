#include "precompiled.hpp"

int* g_pEventCount = nullptr;
dak_event_t* g_pEvents = nullptr;

using DakEvent_AddEvent_t = void(__fastcall*)(int senderId, int receiverId, int type, int itemIdx, float timeStart, float timeEnd, int attachFlags, int a8);
static DakEvent_AddEvent_t pfnDakEvent_AddEvent;

static void __fastcall DakEvent_AddEvent(int senderId, int receiverId, int type, int itemIdx, float timeStart, float timeEnd, int attachFlags, int a8)
{
	if (!pfnDakEvent_AddEvent)
		return;

	//if (g_pEventCount)
	//{
	//	LOG_DBG("[DakEvent_AddEvent][{}] senderId[{}] receiverId[{}] type[{}] a4[{}] timeStart[{}] timeEnd[{}] a7[{}] a7[{}]",
	//		*g_pEventCount, senderId, receiverId, DakEventTypeToStr(EDakEventType(type)), a4, timeStart, timeEnd, a7, a8);
	//}

	pfnDakEvent_AddEvent(senderId, receiverId, type, itemIdx, timeStart, timeEnd, attachFlags, a8);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("8B 3D ?? ?? ?? ?? 81 FF FF 00 00 00", g_pEventCount, 2);
	CREATE_GES("89 8E ?? ?? ?? ?? 8B 4C 24 18 89 96", g_pEvents, 2);
	CREATE_GEH("E8 ?? ?? ?? ?? E9 80 00 00 00 85 FF", pfnDakEvent_AddEvent, DakEvent_AddEvent);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);