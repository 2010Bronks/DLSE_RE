#pragma once

#include "impl/hookchains_impl.h"

#include <utility>
#include <array>
#include <assert.h>
#include <stddef.h>
#include <intrin.h>
#include <Windows.h>

template <typename RetVal, typename... Args>
struct HookChain
{
	using ICallback = IHookChain<RetVal, Args...>;
	using ICallbackRegistry = IHookChainRegistry<RetVal, Args...>;
	using CCallbackRegistry = IHookChainRegistryImpl<RetVal, Args...>;
};

class CHookChainArgs
{
	struct Variant_t
	{
		union
		{
			int8_t S8;
			int16_t S16;
			int32_t S32;
			int64_t S64;

			uint8_t U8;
			uint16_t U16;
			uint32_t U32;
			uint64_t U64;

			float Single;
			double Double;

			void *Pointer;
			char *String;
		};
	};

	enum class eArgType
	{
		Nil,      // std::nullptr_t
		Opaque,   // void*
		Boolean,  // bool
		Signed,   // signed long long
		Unsigned, // unsigned long long
		Single,   // float
		String,   // "payload example string"
	};

	class CArg
	{
	private:
		eArgType _type = eArgType::Nil;
		Variant_t _data = {};

	public:
		CArg() = default;

		eArgType GetType() const;

		void SetPointer(void *value);
		void SetBoolean(bool value);
		void SetSigned(int64_t value);
		void SetUnsigned(uint64_t value);
		void SetSingle(float value);
		void SetString(std::string_view value);

		void *GetPointer() const;
		bool GetBoolean() const;
		int64_t GetSigned() const;
		uint64_t GetUnsigned() const;
		float GetSingle() const;
		std::string_view GetString() const;

		template <typename T>
		std::enable_if_t<std::is_pointer_v<T> && !std::is_same_v<T, const char *> && !std::is_same_v<T, char *>, T> As();

		template <typename T>
		std::enable_if_t<
			!std::is_pointer_v<T> &&
			!std::is_same_v<T, const char *> &&
			!std::is_same_v<T, char *> &&
			!std::is_enum_v<T>,
			T &>
			As();

		template <typename T>
		std::enable_if_t<std::is_enum_v<T>, T &> As();

		template <typename T>
		std::enable_if_t<std::is_same_v<T, const char *> || std::is_same_v<T, char *>, T> As();
	};

	static inline CArg s_dummy_arg = {};

protected:
	size_t _args_count = 0;
	std::array<CArg, 32> _args;

public:
	CHookChainArgs() = default;

	template <typename... Args>
	CHookChainArgs(Args&&... args)
	{
		(push_back(std::forward<Args>(args)), ...);
	}

	template <typename T>
	bool emplace_back(T &&value);

	// Methods for cases when an explicit value needs to be passed,
	// for example, 'emplace_back_signed(0)'.

	bool push_back(const void *value);
	bool push_back(bool value);
	bool push_back(std::nullptr_t);

	template <typename T>
	std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, bool> push_back(T value);

	bool push_back(float value);
	bool push_back(const char *value);

	// std stuff

	const CArg &operator[](size_t index) const;
	CArg &operator[](size_t index);
	const CArg &at(size_t index) const;
	CArg &at(size_t index);

	size_t size() const;
	bool empty() const;

	const CArg *begin() const;
	const CArg *end() const;
	CArg *begin();
	CArg *end();
};

template <typename T>
std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, bool> CHookChainArgs::push_back(T value)
{
	if constexpr (std::is_enum_v<T>)
	{
		using Tu = std::underlying_type_t<T>;

		if constexpr (std::is_signed_v<Tu>)
		{
			return emplace_back(static_cast<int64_t>(static_cast<Tu>(value)));
		}
		else
		{
			return emplace_back(static_cast<uint64_t>(static_cast<Tu>(value)));
		}
	}
	else if constexpr (std::is_signed_v<T>)
	{
		return emplace_back(static_cast<int64_t>(value));
	}
	else if constexpr (std::is_unsigned_v<T>)
	{
		return emplace_back(static_cast<uint64_t>(value));
	}
	else
	{
		assert(false);
		return false;
	}
}

template <typename T>
std::enable_if_t<std::is_pointer_v<T> && !std::is_same_v<T, const char *> && !std::is_same_v<T, char *>, T> CHookChainArgs::CArg::As()
{
	assert(_type == eArgType::Opaque || _type == eArgType::Nil);

	return static_cast<T>(_data.Pointer);
}

template <typename T>
std::enable_if_t<std::is_same_v<T, const char *> || std::is_same_v<T, char *>, T> CHookChainArgs::CArg::As()
{
	assert(_type == eArgType::String || _type == eArgType::Nil);

	return const_cast<T>(_data.String);
}

template <typename T>
std::enable_if_t<
	!std::is_pointer_v<T> &&
	!std::is_same_v<T, const char *> &&
	!std::is_same_v<T, char *> &&
	!std::is_enum_v<T>,
	T &>
	CHookChainArgs::CArg::As()
{
	if constexpr (std::is_same_v<T, bool>)
	{
		assert(_type == eArgType::Boolean);
		return reinterpret_cast<T &>(_data.U8);
	}
	else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>)
	{
		return reinterpret_cast<T &>(_data.S64);
	}
	else if constexpr (std::is_integral_v<T> && std::is_unsigned_v<T>)
	{
		return reinterpret_cast<T &>(_data.U64);
	}
	else if constexpr (std::is_floating_point_v<T>)
	{
		assert(_type == eArgType::Single);
		return reinterpret_cast<T &>(_data.Single);
	}
	else
	{
		static_assert(sizeof(T) == 0, "Unsupported type for CArg::As()");
	}
}

template <typename T>
std::enable_if_t<std::is_enum_v<T>, T &> CHookChainArgs::CArg::As()
{
	using Tu = std::underlying_type_t<T>;

	if constexpr (std::is_signed_v<Tu>)
	{
		return reinterpret_cast<T &>(_data.S64);
	}
	else
	{
		return reinterpret_cast<T &>(_data.U64);
	}
}

template <typename T>
bool CHookChainArgs::emplace_back(T &&value)
{
	using Td = std::decay_t<T>;

	static_assert(
		std::is_pointer_v<Td> ||
		std::is_same_v<Td, bool> ||
		(std::is_integral_v<Td> && std::is_signed_v<Td>) ||
		(std::is_integral_v<Td> && std::is_unsigned_v<Td>) ||
		std::is_floating_point_v<Td> ||
		std::is_same_v<Td, const char *> ||
		std::is_same_v<Td, char *> ||
		std::is_same_v<Td, std::nullptr_t>,
		"Unsupported type for CHookChainArgs::emplace_back"
		);

	assert(_args_count < _args.size());

	if (_args_count >= _args.size())
	{
		std::abort();
	}

	CArg &newArg = _args[_args_count++];

	if constexpr (std::is_same_v<Td, const char *> || std::is_same_v<Td, char *>)
	{
		newArg.SetString(value);
	}
	else if constexpr (std::is_pointer_v<Td>)
	{
		if constexpr (std::is_const_v<std::remove_pointer_t<Td>>)
		{
			newArg.SetPointer(const_cast<void *>(reinterpret_cast<const void *>(std::forward<T>(value))));
		}
		else
		{
			newArg.SetPointer(reinterpret_cast<void *>(std::forward<T>(value)));
		}
	}
	else if constexpr (std::is_same_v<Td, std::nullptr_t>)
	{
		newArg.SetPointer(nullptr);
	}
	else if constexpr (std::is_same_v<Td, bool>)
	{
		newArg.SetBoolean(std::forward<T>(value));
	}
	else if constexpr (std::is_integral_v<Td> && std::is_signed_v<Td>)
	{
		newArg.SetSigned(static_cast<int64_t>(std::forward<T>(value)));
	}
	else if constexpr (std::is_integral_v<Td> && std::is_unsigned_v<Td>)
	{
		newArg.SetUnsigned(static_cast<uint64_t>(std::forward<T>(value)));
	}
	else if constexpr (std::is_floating_point_v<Td>)
	{
		newArg.SetSingle(static_cast<float>(std::forward<T>(value)));
	}
	else
	{
		std::abort();
	}

	return true;
}

struct RegisterCallback
{
	template <class T>
	explicit RegisterCallback(T &chain, T::hookfunc_t func, const HookChainRegistryMeta &meta = HookChainRegistryMeta())
	{
		chain.registerHook(func, HC_PRIORITY_DEFAULT, meta);
	}

	template <class T>
	explicit RegisterCallback(T &chain, T::hookfunc_t func, size_t priority, const HookChainRegistryMeta &meta = HookChainRegistryMeta())
	{
		chain.registerHook(func, priority, meta);
	}
};

struct ExecuteCallback
{
	explicit ExecuteCallback(void(*callback)())
	{
		assert(callback);

		if (callback)
			callback();
	}
};

#define CB_COMBINE1(X, Y) X##Y 
#define CB_COMBINE(X, Y) CB_COMBINE1(X, Y)

#define REGISTER_CALLBACK(chain, func, ...) static auto CB_COMBINE(__RegCb_ ## chain ## _, __LINE__) = RegisterCallback(CCallbackMgr:: ## chain(), func, __VA_ARGS__, HookChainRegistryMeta())
#define EXECUTE_CALLBACK(func) static auto CB_COMBINE(__ExecCb_, __LINE__) = ExecuteCallback(func)