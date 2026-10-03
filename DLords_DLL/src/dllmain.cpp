#include "precompiled.hpp"

// D9 05 ? ? ? ? C7 04 BD

#define GAME_NAME "DLSteamEdition.exe"

std::unique_ptr<U::Memory::module_t> gGameModule;

static bool Initialize()
{
	Logger::Init();

	LOG("Initialization process started...");

	gGameModule = std::make_unique<U::Memory::module_t>(GAME_NAME);

	if (gGameModule->GetLoaded())
	{
		LOG("Module [{}] found. Base addr {}, size {}", gGameModule->GetName(), gGameModule->GetBase(), gGameModule->GetSize());
	}
	else
	{
		LOG("Module [{}] could not be found, exiting...");
		return false;
	}

	//CHookChainArgs_Init init_args;
	CCallbackMgr::Init().Run();

	return true;
}

static DWORD CALLBACK Main(const thread_info_t& info)
{
	if (info.isWorking)
	{
		if (!Initialize())
			return 0;

		while (info.isWorking)
		{
			std::this_thread::sleep_for(1ms);
			CCallbackMgr::OnDllFrame().Run();
		}
	}
	else
	{
		for (auto&& thread : CThread::GetThreads())
		{
			thread->Stop();
		}
	}

	return 0;
}

auto gMainThread = CThread("Main", Main, false);
HINSTANCE ghInstDLL{};

BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		ghInstDLL = hinstDLL;
		gMainThread.Start();
	}

	if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		gMainThread.Stop();
		CCallbackMgr::Done().Run();
		Logger::Done();
	}

	return TRUE;
}

