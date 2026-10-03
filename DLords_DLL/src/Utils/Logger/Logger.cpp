#include "precompiled.hpp"

namespace
{
	std::mutex g_LogMutex;
	HANDLE g_StdOut = nullptr;
	WORD g_DefaultAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
	bool g_ConsoleAllocated = false;
	bool g_HandlerRegistered = false;

	std::string GetLogTime()
	{
		const time_t nowTime = time(nullptr);
		tm now{};
		localtime_s(&now, &nowTime);
		return std::format("{:0>2}.{:0>2}.{:0>4} {:0>2}:{:0>2}:{:0>2}", now.tm_mday, now.tm_mon + 1, now.tm_year + 1900, now.tm_hour, now.tm_min, now.tm_sec);
	}

	const char* GetLevelName(Logger::Level level)
	{
		switch (level)
		{
		case Logger::Level::Debug: return "DEBUG";
		case Logger::Level::Success: return "SUCCESS";
		case Logger::Level::Important: return "IMPORTANT";
		case Logger::Level::Warning: return "WARNING";
		case Logger::Level::Error: return "ERROR";
		default: return "";
		}
	}

	WORD GetLevelColor(Logger::Level level)
	{
		switch (level)
		{
		case Logger::Level::Debug: return FOREGROUND_RED | FOREGROUND_GREEN;
		case Logger::Level::Success: return FOREGROUND_GREEN;
		case Logger::Level::Important: return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		case Logger::Level::Warning: return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		case Logger::Level::Error: return FOREGROUND_RED | FOREGROUND_INTENSITY;
		default: return g_DefaultAttributes;
		}
	}

	void WriteConsole(std::string_view text)
	{
		if (!g_StdOut || text.empty())
			return;

		DWORD written = 0;
		WriteFile(g_StdOut, text.data(), static_cast<DWORD>(text.size()), &written, nullptr);
	}

	BOOL WINAPI ConsoleHandlerRoutine(DWORD ctrlType)
	{
		if (ctrlType == CTRL_C_EVENT || ctrlType == CTRL_BREAK_EVENT)
		{
			LOG_IMPORTANT("[Console] Break signal has been blocked.");
			return TRUE;
		}

		return FALSE;
	}
}

void Logger::Init()
{
	std::lock_guard lock(g_LogMutex);
	if (g_StdOut)
		return;

	DWORD processId = 0;
	const HWND consoleWindow = GetConsoleWindow();
	if (consoleWindow)
		GetWindowThreadProcessId(consoleWindow, &processId);

	if (!consoleWindow || processId != GetCurrentProcessId())
	{
		if (!AllocConsole())
			return;

		g_ConsoleAllocated = true;
		SetConsoleTitleA("Dungeon Lords Steam Edition Modification");
	}

	g_StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!g_StdOut || g_StdOut == INVALID_HANDLE_VALUE)
	{
		g_StdOut = nullptr;
		if (g_ConsoleAllocated)
		{
			FreeConsole();
			g_ConsoleAllocated = false;
		}
		return;
	}

	CONSOLE_SCREEN_BUFFER_INFO info{};
	if (GetConsoleScreenBufferInfo(g_StdOut, &info))
		g_DefaultAttributes = info.wAttributes;

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	g_HandlerRegistered = SetConsoleCtrlHandler(&ConsoleHandlerRoutine, TRUE) != FALSE;
}

void Logger::Done()
{
	std::lock_guard lock(g_LogMutex);
	if (g_HandlerRegistered)
	{
		SetConsoleCtrlHandler(&ConsoleHandlerRoutine, FALSE);
		g_HandlerRegistered = false;
	}

	g_StdOut = nullptr;
	if (g_ConsoleAllocated)
	{
		FreeConsole();
		g_ConsoleAllocated = false;
	}
}

void Logger::Write(Level level, std::string_view text)
{
	std::lock_guard lock(g_LogMutex);
	if (!g_StdOut || text.empty())
		return;

	const auto prefix = std::format("{} | ", GetLogTime());
	const auto levelText = std::format("{:<9}", GetLevelName(level));
	const auto suffix = std::format(" | {}\n", text);

	WriteConsole(prefix);
	if (level != Level::Info)
		SetConsoleTextAttribute(g_StdOut, GetLevelColor(level));
	WriteConsole(levelText);
	if (level != Level::Info)
		SetConsoleTextAttribute(g_StdOut, g_DefaultAttributes);
	WriteConsole(suffix);
}
