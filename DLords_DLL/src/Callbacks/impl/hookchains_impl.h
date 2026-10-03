#pragma once

#include "hookchains.h"

#include <source_location>

class HookChainItem
{
private:
	bool _active;
	int _priority;
	void *_hook;
	HookChainRegistryMeta _meta;

public:
	HookChainItem() = delete;

	HookChainItem(void *hookFunc, int priority, const HookChainRegistryMeta &meta)
		: _active(true)
		, _priority(priority)
		, _hook(hookFunc)
		, _meta(meta)
	{

	}

	void set_state(bool state) { _active = state; }
	void set_priority(int value) { _priority = value; }

	bool active() const { return _active; }

	void *hook() const { return _hook; }

	int priority() const { return _priority; }

	const auto &meta() const { return _meta; }
};

// Implementation for chains in modules
template<typename t_ret, typename ...t_args>
class IHookChainImpl : public IHookChain<t_ret, t_args...>
{
public:
	typedef t_ret(*hookfunc_t)(IHookChain<t_ret, t_args...> *, t_args&&...);
	typedef t_ret(*origfunc_t)(t_args&&...);

	IHookChainImpl(HookChainItem *hooks, origfunc_t orig) : m_Hooks(hooks), m_OriginalFunc(orig)
	{
		
	}

	virtual ~IHookChainImpl() {}

	virtual t_ret callNext(t_args&&... args)
	{
		while (!m_Hooks[0].active())
			m_Hooks++;

		hookfunc_t nexthook = (hookfunc_t)m_Hooks[0].hook();

		if (nexthook)
		{
			IHookChainImpl nextChain(m_Hooks + 1, m_OriginalFunc);
			return nexthook(&nextChain, std::forward<t_args>(args)...);
		}

		return m_OriginalFunc ? m_OriginalFunc(std::forward<t_args>(args)...) : t_ret();
	}

	virtual t_ret callOriginal(t_args&&... args)
	{
		return m_OriginalFunc ? m_OriginalFunc(std::forward<t_args>(args)...) : t_ret();
	}

private:
	HookChainItem *m_Hooks;
	origfunc_t m_OriginalFunc;
};

// Implementation for chains in modules
template <typename t_ret, typename t_class, typename ...t_args>
class IHookChainClassImpl : public IHookChainClass<t_ret, t_class, t_args...>
{
public:
	typedef t_ret(*hookfunc_t)(IHookChainClass<t_ret, t_class, t_args...> *, t_class *, t_args&&...);
	typedef t_ret(t_class:: *origfunc_t)(t_args&&...);

	IHookChainClassImpl(HookChainItem *hooks, origfunc_t orig) : m_Hooks(hooks), m_OriginalFunc(orig)
	{
		if (orig == nullptr && !is_void(orig))
			assert(0 && "Non-void HookChain without original function.");
	}

	virtual ~IHookChainClassImpl() {}

	virtual t_ret callNext(t_class *object, t_args&&... args)
	{
		while (!m_Hooks[0].active())
			m_Hooks++;

		hookfunc_t nexthook = (hookfunc_t)m_Hooks[0].hook();

		if (nexthook)
		{
			IHookChainClassImpl nextChain(m_Hooks + 1, m_OriginalFunc);
			return nexthook(&nextChain, object, std::forward<t_args>(args)...);
		}

		return m_OriginalFunc ? (object->*m_OriginalFunc)(std::forward<t_args>(args)...) : t_ret();
	}

	virtual t_ret callOriginal(t_class *object, t_args&&... args)
	{
		return m_OriginalFunc ? (object->*m_OriginalFunc)(std::forward<t_args>(args)...) : t_ret();
	}

private:
	HookChainItem *m_Hooks;
	origfunc_t m_OriginalFunc;
};

// Implementation for chains in modules
template <typename t_ret, typename t_class, typename ...t_args>
class IHookChainClassEmptyImpl : public IHookChain<t_ret, t_args...>
{
public:
	typedef t_ret(*hookfunc_t)(IHookChain<t_ret, t_args...> *, t_args&&...);
	typedef t_ret(t_class:: *origfunc_t)(t_args&&...);

	IHookChainClassEmptyImpl(void **hooks, origfunc_t orig, t_class *object) 
		: m_Hooks(hooks)
		, m_Object(object)
		, m_OriginalFunc(orig)
	{
		if (orig == nullptr && !is_void(orig))
			assert(0 && "Non-void HookChain without original function.");
	}

	virtual ~IHookChainClassEmptyImpl() {}

	virtual t_ret callNext(t_args&&... args)
	{
		while (!m_Hooks[0].active())
			m_Hooks++;

		hookfunc_t nexthook = (hookfunc_t)m_Hooks[0].hook();

		if (nexthook)
		{
			IHookChainClassEmptyImpl nextChain(m_Hooks + 1, m_OriginalFunc, m_Object);
			return nexthook(&nextChain, std::forward<t_args>(args)...);
		}

		return m_OriginalFunc ? (m_Object->*m_OriginalFunc)(std::forward<t_args>(args)...) : t_ret();
	}

	virtual t_ret callOriginal(t_args&&... args)
	{
		return m_OriginalFunc ? (m_Object->*m_OriginalFunc)(std::forward<t_args>(args)...) : t_ret();
	}

private:
	HookChainItem *m_Hooks;
	t_class *m_Object;
	origfunc_t m_OriginalFunc;
};

class AbstractHookChainRegistry
{
protected:
	std::vector<HookChainItem> _chain;

protected:
	void add(void *hookFunc, int priority, const HookChainRegistryMeta &meta = HookChainRegistryMeta());
	void erase(void *hookFunc);
	void sort();

public:
	AbstractHookChainRegistry();

	std::vector<HookChainItem> &chain() { return _chain; }
};

template<typename t_ret, typename ...t_args>
class IHookChainRegistryImpl : public IHookChainRegistry < t_ret, t_args...>, public AbstractHookChainRegistry
{
public:
	typedef t_ret(*hookfunc_t)(IHookChain<t_ret, t_args...> *, t_args&&...);
	typedef t_ret(*origfunc_t)(t_args&&...);

	virtual ~IHookChainRegistryImpl() {}

	t_ret RunOrigin(origfunc_t origFunc)
	{
		std::tuple<std::decay_t<t_args>...> args{};
		IHookChainImpl<t_ret, t_args...> chain(_chain.data(), origFunc);

		return std::apply([&chain](auto&&... unpackedArgs)
			{
				return chain.callNext(std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
			}, args);
	}

	t_ret RunOrigin(origfunc_t origFunc, t_args&&... args)
	{
		IHookChainImpl<t_ret, t_args...> chain(_chain.data(), origFunc);
		return chain.callNext(std::forward<t_args>(args)...);
	}

	t_ret Run(t_args&&... args)
	{
		return RunOrigin(nullptr, std::forward<t_args>(args)...);
	}
	
	t_ret Run()
	{
		std::tuple<std::decay_t<t_args>...> args{};

		return std::apply([this](auto&&... unpackedArgs)
			{
				return RunOrigin(nullptr, std::forward<decltype(unpackedArgs)>(unpackedArgs)...);
			}, args);
	}

	virtual void registerHook(hookfunc_t hook, size_t priority = HC_PRIORITY_DEFAULT, const HookChainRegistryMeta &meta = HookChainRegistryMeta())
	{
		add((void *)hook, (int)priority, meta);
	}

	virtual void unregisterHook(hookfunc_t hook)
	{
		erase((void *)hook);
	}
};

template <typename t_ret, typename t_class, typename ...t_args>
class IHookChainRegistryClassImpl : public IHookChainRegistryClass<t_ret, t_class, t_args...>, public AbstractHookChainRegistry
{
public:
	typedef t_ret(*hookfunc_t)(IHookChainClass<t_ret, t_class, t_args...> *, t_class *, t_args&&...);
	typedef t_ret(t_class:: *origfunc_t)(t_args&&...);

	virtual ~IHookChainRegistryClassImpl() {}

	t_ret callChain(origfunc_t origFunc, t_class* object, t_args&&... args)
	{
		IHookChainClassImpl<t_ret, t_class, t_args...> chain(_chain.data(), origFunc);
		return chain.callNext(object, std::forward<t_args>(args)...);
	}

	virtual void registerHook(hookfunc_t hook, int priority = HC_PRIORITY_DEFAULT, const HookChainRegistryMeta &meta = HookChainRegistryMeta())
	{
		add((void *)hook, priority, meta);
	}

	virtual void unregisterHook(hookfunc_t hook)
	{
		erase((void *)hook);
	}
};

template <typename t_ret, typename t_class, typename ...t_args>
class IHookChainRegistryClassEmptyImpl : public IHookChainRegistry<t_ret, t_args...>, public AbstractHookChainRegistry
{
public:
	typedef t_ret(*hookfunc_t)(IHookChain<t_ret, t_args...> *, t_args&&...);
	typedef t_ret(t_class:: *origfunc_t)(t_args&&...);

	virtual ~IHookChainRegistryClassEmptyImpl() {}

	t_ret callChain(origfunc_t origFunc, t_class* object, t_args&&... args)
	{
		IHookChainClassEmptyImpl<t_ret, t_class, t_args...> chain(_chain.data(), origFunc, object);
		return chain.callNext(std::forward<t_args>(args)...);
	}

	virtual void registerHook(hookfunc_t hook, int priority = HC_PRIORITY_DEFAULT, const HookChainRegistryMeta &meta = HookChainRegistryMeta())
	{
		add((void *)hook, priority, meta);
	}

	virtual void unregisterHook(hookfunc_t hook)
	{
		erase((void *)hook);
	}
};