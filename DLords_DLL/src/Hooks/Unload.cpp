#include "precompiled.hpp"

extern CSimpleUI gSimpleUI;

static bool gbUnloadProductRequested = false;

void RequestUnload()
{
	gbUnloadProductRequested = true;
}

bool IsUnloadRequested()
{
	return gbUnloadProductRequested;
}

extern HINSTANCE ghInstDLL;
static DWORD CALLBACK UnloadCallback(LPVOID lpThreadParameter)
{
	LOG_DBG("Freeing product memory from thread.");

	for (auto thread : CThread::GetThreads())
		thread->Stop();

	CGameHookMgr::Instance().RemoveAllHooks();
	U::Memory::RestoreMemoryBackups();
	LOG_DBG("Product memory has been released.");
	Logger::Done();
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(ghInstDLL), 0);
}

static void PerformProductUnload()
{
	LOG_DBG("Performing product unlinking.");

	if (gSimpleUI.IsOpened())
		gSimpleUI.Switch();

	HANDLE thread = CreateThread(nullptr, 0, UnloadCallback, nullptr, 0, nullptr);
	if (thread)
		CloseHandle(thread);
}

static void OnDllFrame(HookChain_OnDllFrame::ICallback* chain, CHookChainArgs_OnDllFrame& args)
{
	if (gbUnloadProductRequested)
	{
		static bool ensure_unload = (PerformProductUnload(), true);
	}

	chain->callNext(args);
}

REGISTER_CALLBACK(OnDllFrame, OnDllFrame);
