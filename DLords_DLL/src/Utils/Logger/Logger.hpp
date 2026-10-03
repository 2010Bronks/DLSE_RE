#pragma once

#include <format>
#include <string_view>
#include <utility>

namespace Logger
{
	enum class Level
	{
		Info,
		Debug,
		Success,
		Important,
		Warning,
		Error
	};

	void Init();
	void Done();
	void Write(Level level, std::string_view text);

	inline void Log(Level level, std::string_view text)
	{
		if (!text.empty())
			Write(level, text);
	}

	template <typename... Args>
	void Log(Level level, std::format_string<Args...> fmt, Args&&... args)
	{
		Write(level, std::format(fmt, std::forward<Args>(args)...));
	}
}

#ifdef LOG_ENABLE
	#define LOG(...) ::Logger::Log(::Logger::Level::Info, __VA_ARGS__)
	#define LOG_SUCCESS(...) ::Logger::Log(::Logger::Level::Success, __VA_ARGS__)
	#define LOG_IMPORTANT(...) ::Logger::Log(::Logger::Level::Important, __VA_ARGS__)
	#define LOG_WARNING(...) ::Logger::Log(::Logger::Level::Warning, __VA_ARGS__)
	#define LOG_ERROR(...) ::Logger::Log(::Logger::Level::Error, __VA_ARGS__)

	#ifdef _DEBUG
		#define LOG_DBG(...) ::Logger::Log(::Logger::Level::Debug, __VA_ARGS__)
		#define LOG_DBG_SUCCESS(...) ::Logger::Log(::Logger::Level::Success, __VA_ARGS__)
		#define LOG_DBG_IMPORTANT(...) ::Logger::Log(::Logger::Level::Important, __VA_ARGS__)
		#define LOG_DBG_WARNING(...) ::Logger::Log(::Logger::Level::Warning, __VA_ARGS__)
		#define LOG_DBG_ERROR(...) ::Logger::Log(::Logger::Level::Error, __VA_ARGS__)
	#else
		#define LOG_DBG(...) ((void)0)
		#define LOG_DBG_SUCCESS(...) ((void)0)
		#define LOG_DBG_IMPORTANT(...) ((void)0)
		#define LOG_DBG_WARNING(...) ((void)0)
		#define LOG_DBG_ERROR(...) ((void)0)
	#endif
#else
	#define LOG(...) ((void)0)
	#define LOG_SUCCESS(...) ((void)0)
	#define LOG_IMPORTANT(...) ((void)0)
	#define LOG_WARNING(...) ((void)0)
	#define LOG_ERROR(...) ((void)0)
	#define LOG_DBG(...) ((void)0)
	#define LOG_DBG_SUCCESS(...) ((void)0)
	#define LOG_DBG_IMPORTANT(...) ((void)0)
	#define LOG_DBG_WARNING(...) ((void)0)
	#define LOG_DBG_ERROR(...) ((void)0)
#endif
