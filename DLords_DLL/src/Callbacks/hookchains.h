#pragma once

#include <optional>
#include <source_location>
#include <filesystem>
#include <assert.h>

template <typename t_ret, typename t_class, typename ...t_args>
bool is_void(t_ret(t_class:: *)(t_args...)) { return false; }

template <typename t_ret, typename ...t_args>
bool is_void(t_ret(*)(t_args...)) { return false; }

template <typename t_class, typename ...t_args>
bool is_void(void (t_class:: *)(t_args...)) { return true; }

template <typename ...t_args>
bool is_void(void (*)(t_args...)) { return true; }

template<typename t_ret, typename ...t_args>
class IHookChain {
protected:
	virtual ~IHookChain() {}

public:
	virtual t_ret callNext(t_args&&... args) = 0;
	virtual t_ret callOriginal(t_args&&... args) = 0;
};

template<typename t_ret, typename t_class, typename ...t_args>
class IHookChainClass {
protected:
	virtual ~IHookChainClass() {}

public:
	virtual t_ret callNext(t_class *, t_args&&... args) = 0;
	virtual t_ret callOriginal(t_class *, t_args&&... args) = 0;
};

// Specifies priorities for hooks call order in the chain.
// For equal priorities first registered hook will be called first.
enum HookChainPriority
{
	HC_PRIORITY_UNINTERRUPTABLE = 255,  // Hook will be called before other hooks.
	HC_PRIORITY_HIGH = 192,             // Hook will be called before hooks with default priority.
	HC_PRIORITY_DEFAULT = 128,          // Default hook call priority.
	HC_PRIORITY_MEDIUM = 64,            // Hook will be called after hooks with default priority.
	HC_PRIORITY_LOW = 0,                // Hook will be called after all other hooks.
};

class HookChainRegistryMeta
{
private:
	std::source_location _meta;

public:
#ifdef _DEBUG
	__forceinline HookChainRegistryMeta(std::source_location source_loc = std::source_location::current())
		: _meta(source_loc)
	{

	}
#else
	__forceinline HookChainRegistryMeta()
		: _meta()
	{

	}
#endif

	auto source_column() const { return _meta.column(); }
	auto source_line() const { return _meta.line(); }
	std::string source_filename() const { return std::filesystem::path(_meta.file_name()).filename().string(); }
	std::string source_funcname() const { return _meta.function_name(); }
};

// Hook chain registry(for hooks [un]registration)
template<typename t_ret, typename ...t_args>
class IHookChainRegistry
{
public:
	typedef t_ret(*hookfunc_t)(IHookChain<t_ret, t_args...>*, t_args&&...);

	virtual void registerHook(hookfunc_t hook, size_t priority = HC_PRIORITY_DEFAULT, const HookChainRegistryMeta &src = HookChainRegistryMeta()) = 0;
	virtual void unregisterHook(hookfunc_t hook) = 0;
};

// Hook chain registry(for hooks [un]registration)
template<typename t_ret, typename t_class, typename ...t_args>
class IHookChainRegistryClass
{
public:
	typedef t_ret(*hookfunc_t)(IHookChainClass<t_ret, t_class, t_args...> *, t_class *, t_args&&...);

	virtual void registerHook(hookfunc_t hook, size_t priority = HC_PRIORITY_DEFAULT, const HookChainRegistryMeta &src = HookChainRegistryMeta()) = 0;
	virtual void unregisterHook(hookfunc_t hook) = 0;
};