#include "precompiled.hpp"

namespace WorldTime
{
	bool bLog = true;

	unsigned int ComputeTotalSeconds(unsigned int days, unsigned int hours, unsigned int minutes)
	{
		unsigned int totalMinutes = minutes + 60 * (hours + 24 * days);
		return totalMinutes * 60;
	}

	void ExtractTimeFromSeconds(unsigned int totalSeconds, unsigned int& days, unsigned int& hours, unsigned int& minutes)
	{
		unsigned int totalMinutes = totalSeconds / 60;
		unsigned int totalHours = totalMinutes / 60;

		minutes = totalMinutes % 60;
		hours = totalHours % 24;
		days = totalHours / 24;
	}
}

namespace WorldTime
{
	using SetTotalSeconds_t = void(__fastcall*)(int totalSeconds);

	static int* s_pTimeTotalSeconds = nullptr;
	static SetTotalSeconds_t pfnSetTotalSeconds;

	static void __fastcall SetTotalSeconds(int totalSeconds)
	{
		if (!pfnSetTotalSeconds)
			return;

		if (bLog)
		{
			unsigned int days, hours, mins;
			ExtractTimeFromSeconds(totalSeconds, days, hours, mins);
			LOG("[WorldTime::SetTotalSeconds] totalSeconds[{}]: days[{}] hours[{}] minutes[{}]", totalSeconds, days, hours, mins);
		}

		pfnSetTotalSeconds(totalSeconds);
	}

	static void Find()
	{
		CREATE_GES("8B 0D ?? ?? ?? ?? 83 C4 04 E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 33 F6 39 3D", s_pTimeTotalSeconds, 2);

		CREATE_GEH("E8 ?? ?? ?? ?? E8 ?? ?? ?? ?? 33 F6 39 3D ?? ?? ?? ?? 7E 23", pfnSetTotalSeconds, SetTotalSeconds);
	}

	void Set(unsigned int days, unsigned int hours, unsigned int minutes)
	{
		if (!pfnSetTotalSeconds)
		{
			LOG_WARNING("[WorldTime::Set] pfnSetTotalSeconds not found!");
			return;
		}

		pfnSetTotalSeconds(ComputeTotalSeconds(days, hours, minutes));
	}

	bool Get(unsigned int &days, unsigned int &hours, unsigned int &minutes)
	{
		if (!s_pTimeTotalSeconds)
		{
			LOG_WARNING("[WorldTime::Get] s_pTimeTotalSeconds not found!");

			days = hours = minutes = 0;
			return false;
		}

		ExtractTimeFromSeconds(*s_pTimeTotalSeconds, days, hours, minutes);
		return true;
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	WorldTime::Find();

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);