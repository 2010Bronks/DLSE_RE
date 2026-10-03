#include "precompiled.hpp"

bool renderer_data_t::IsReady() const
{
	return pD3d && pD3d->IsReady();
}

IDirect3DDevice9* renderer_data_t::GetDevice()
{
	if (!IsReady())
		return nullptr;

	return pD3d->pDevice;
}

bool d3d_data_t::IsReady() const
{
	return pData && pDevice;
}