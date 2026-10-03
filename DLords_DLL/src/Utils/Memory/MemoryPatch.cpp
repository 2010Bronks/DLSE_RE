#include "precompiled.hpp"

bool Memoria::CMemoryPatch::IsActive() const
{
	if (!_dest_address)
		return false;

	return _active;
}

bool Memoria::CMemoryPatch::IsValid() const
{
	if (!_dest_address)
		return false;

	bool equal;

	if (_active)
		equal = std::memcmp(_dest_address, _data_patch.data(), _data_patch.size()) == 0;
	else
		equal = std::memcmp(_dest_address, _data_origin.data(), _data_origin.size()) == 0;

	return equal;
}

void Memoria::CMemoryPatch::Apply()
{
	if (_data_origin.empty() || _data_patch.empty())
		return;

	if (!IsActive())
		Toggle(true);
}

void Memoria::CMemoryPatch::Restore()
{
	if (_data_origin.empty() || _data_patch.empty())
		return;

	if (IsActive())
		Toggle(false);
}

void Memoria::CMemoryPatch::Toggle(bool state)
{
	if (_data_origin.empty() || _data_patch.empty())
		return;

	if (_modify_protect)
		Memoria::PushMemoryProtection(_dest_address, _data_origin.size(), PAGE_EXECUTE_READWRITE);

	if (state)
		std::memcpy(_dest_address, _data_patch.data(), _data_patch.size());
	else
		std::memcpy(_dest_address, _data_origin.data(), _data_origin.size());

	if (_modify_protect)
		Memoria::PopMemoryProtection(_dest_address);

	_active = state;
}

Memoria::CMemoryPatch::CMemoryPatch(void* dest_address, const void* source_address, size_t size, bool modify_prot)
	: _dest_address(dest_address)
	, _modify_protect(modify_prot)
	, _active(false)
	, _stack_backtrace(GetStackBacktrace())
{
	assert(dest_address && source_address && size);

	if (dest_address && source_address && size > 0)
	{
		_data_origin.resize(size);
		_data_patch.resize(size);

		std::memcpy(_data_origin.data(), dest_address, size);
		std::memcpy(_data_patch.data(), source_address, size);
	}
	else
	{
		_dest_address = {};

		_data_origin.clear();
		_data_patch.clear();
	}
}

Memoria::CMemoryPatch::~CMemoryPatch()
{
	LOG_DBG("MemoryPatch: Address {} ({}) restored.", Memoria::BeautifyPointer(_dest_address), _dest_address);

	if (IsActive())
		Toggle(false);
}