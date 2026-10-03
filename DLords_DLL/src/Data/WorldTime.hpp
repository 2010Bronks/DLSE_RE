#pragma once

namespace WorldTime
{
	extern bool bLog;

	extern unsigned int ComputeTotalSeconds(unsigned int days, unsigned int hours, unsigned int minutes);
	extern void ExtractTimeFromSeconds(unsigned int totalSeconds, unsigned int& days, unsigned int& hours, unsigned int& minutes);

	extern void Set(unsigned int days, unsigned int hours, unsigned int minutes);
	extern bool Get(unsigned int& days, unsigned int& hours, unsigned int& minutes);
}