// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <precompiled.hpp>
#include "hookchains.h"
#include "callbacks.hpp"

const CHookChainArgs::CArg& CHookChainArgs::operator[](size_t index) const
{
	if (index >= _args_count)
	{
		assert(false);
		s_dummy_arg = {};
		return s_dummy_arg;
	}

	return _args[index];
}

CHookChainArgs::CArg& CHookChainArgs::operator[](size_t index)
{
	if (index >= _args_count)
	{
		assert(false);
		s_dummy_arg = {};
		return s_dummy_arg;
	}

	return _args[index];
}

const CHookChainArgs::CArg& CHookChainArgs::at(size_t index) const
{
	if (index >= _args_count)
	{
		assert(false);
		s_dummy_arg = {};
		return s_dummy_arg;
	}

	return _args[index];
}

CHookChainArgs::CArg& CHookChainArgs::at(size_t index)
{
	if (index >= _args_count)
	{
		assert(false);
		s_dummy_arg = {};
		return s_dummy_arg;
	}

	return _args[index];
}

size_t CHookChainArgs::size() const
{
	return _args_count;
}

bool CHookChainArgs::empty() const
{
	return _args_count == 0;
}

CHookChainArgs::eArgType CHookChainArgs::CArg::GetType() const
{
	return _type;
}

void CHookChainArgs::CArg::SetPointer(void *value)
{
	if (value == nullptr)
	{
		_type = eArgType::Nil;
	}
	else
	{
		_type = eArgType::Opaque;
	}

	_data.Pointer = value;
}

void CHookChainArgs::CArg::SetBoolean(bool value)
{
	_type = eArgType::Boolean;
	_data.U64 = value;
}

void CHookChainArgs::CArg::SetSigned(int64_t value)
{
	_type = eArgType::Signed;
	_data.S64 = value;
}

void CHookChainArgs::CArg::SetUnsigned(uint64_t value)
{
	_type = eArgType::Unsigned;
	_data.U64 = value;
}

void CHookChainArgs::CArg::SetSingle(float value)
{
	_type = eArgType::Single;
	_data.Single = value;
}

void CHookChainArgs::CArg::SetString(std::string_view value)
{
	if (value.data() == nullptr)
	{
		_type = eArgType::Nil;
	}
	else
	{
		_type = eArgType::String;
	}

	_data.String = const_cast<char *>(value.data());
}

void *CHookChainArgs::CArg::GetPointer() const
{
	return _data.Pointer;
}

bool CHookChainArgs::CArg::GetBoolean() const
{
	return _data.S64 > 0;
}

int64_t CHookChainArgs::CArg::GetSigned() const
{
	return _data.S64;
}

uint64_t CHookChainArgs::CArg::GetUnsigned() const
{
	return _data.U64;
}

float CHookChainArgs::CArg::GetSingle() const
{
	return _data.Single;
}

std::string_view CHookChainArgs::CArg::GetString() const
{
	return _data.String;
}

bool CHookChainArgs::push_back(const void *value)
{
	return emplace_back(value);
}

bool CHookChainArgs::push_back(bool value)
{
	return emplace_back(value);
}

bool CHookChainArgs::push_back(std::nullptr_t)
{
	return emplace_back(nullptr);
}

bool CHookChainArgs::push_back(float value)
{
	return emplace_back(value);
}

bool CHookChainArgs::push_back(const char *value)
{
	return emplace_back(value);
}

const CHookChainArgs::CArg* CHookChainArgs::begin() const
{
	return _args.data();
}

const CHookChainArgs::CArg* CHookChainArgs::end() const
{
	return _args.data() + _args_count;
}

CHookChainArgs::CArg* CHookChainArgs::begin()
{
	return _args.data();
}

CHookChainArgs::CArg* CHookChainArgs::end()
{
	return _args.data() + _args_count;
}