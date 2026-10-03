#include "precompiled.hpp"

/*
	While using the teleport function, you may accidentally enter
	out-of-bounds space. After that, the game shows an error message
	about it. We simply disable that message, so this never happens.
*/

bool gbFixOOBCrash = true;

using LogError_t = void(__fastcall*)(const char* szMsg, int unk);
static LogError_t pfnLogError = nullptr;
static void __fastcall LogError(const char* szMsg, int unk)
{
	if (!pfnLogError)
		return;

	if (gbFixOOBCrash)
		return;

	pfnLogError(szMsg, unk);
}
static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ?? ?? ?? ?? 5F 5D 33 C0", pfnLogError, LogError);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);