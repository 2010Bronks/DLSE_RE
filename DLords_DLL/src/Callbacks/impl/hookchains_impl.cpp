// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <precompiled.hpp>
#include "hookchains_impl.h"

AbstractHookChainRegistry::AbstractHookChainRegistry()
	: _chain()
{
	_chain.emplace_back(nullptr, -1, HookChainRegistryMeta());
}

void AbstractHookChainRegistry::add(void *hookFunc, int priority, const HookChainRegistryMeta &meta)
{
	if (!hookFunc)
	{
		assert(!"Parameter hookFunc can't be a nullptr");
		return;
	}

	HookChainItem newItem(hookFunc, priority, meta);

	auto it = std::lower_bound
	(
		_chain.begin(),
		_chain.end(),
		priority,
		[](const HookChainItem &item, int priority)
		{
			return item.priority() > priority;
		}
	);

	_chain.insert(it, std::move(newItem));
}

void AbstractHookChainRegistry::erase(void *hookFunc)
{
	if (!hookFunc)
	{
		assert(!"Parameter hookFunc can't be a nullptr");
		return;
	}

	auto it = std::remove_if
	(
		_chain.begin(),
		_chain.end(),
		[hookFunc](const HookChainItem &item)
		{
			return item.hook() == hookFunc;
		}
	);

	_chain.erase(it, _chain.end());
}

void AbstractHookChainRegistry::sort()
{
	std::sort(_chain.begin(), _chain.end(), [](const HookChainItem &a, const HookChainItem &b)
		{
			return a.priority() > b.priority();
		});
}