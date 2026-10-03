#pragma once

#include "SimpleUI/Widgets/Widgets.hpp"
#include "SimpleUI/SimpleUI.hpp"
#include "Drawing/Drawing.hpp"

struct CGameRendererStates
{
private:
	renderer_states_t* m_pData;
	renderer_states_t m_internalData;

	bool m_bInited;

public:
	CGameRendererStates();

	void Init(renderer_states_t* pStates);
	renderer_states_t& GetData();
	bool SetData();
};

extern CGameRendererStates gRenderStates;

extern renderer_data_t* g_pGameRenderer;
extern renderer_states_t* g_pGameRendererStates;

enum class EFogVar
{
	enable,
	range_based,
	color,
	mode,
	mode_vertex,
	start,
	end,
	density,
};

class CFogManager
{
private:
	IDirect3DDevice9* m_pDevice;

private:
	bool m_bReady;

private:
	void SetInternal(_D3DRENDERSTATETYPE type, DWORD* value)
	{
		if (!m_pDevice)
			return;

		m_pDevice->SetRenderState(type, *value);
	}

public:
	bool enable;
	bool range_based;
	ImU32 color;
	int mode;
	int mode_vertex;
	float start;
	float end;
	float density;

public:
	CFogManager() : m_pDevice(nullptr), m_bReady(false)
	{
		enable = false;
		range_based = false;
		color = 0;
		mode = 0;
		mode_vertex = 0;
		start = 0.0f;
		end = 0.0f;
		density = 0.0f;
	}

	void Init(IDirect3DDevice9* pDevice)
	{
		if (!pDevice)
		{
			m_pDevice = nullptr;
			m_bReady = false;
			return;
		}

		m_pDevice = pDevice;
		m_bReady = true;
	}

	void Update(EFogVar var)
	{
		return;

		if (!m_bReady)
			return;

		static _D3DFOGMODE mode_max = D3DFOG_FORCE_DWORD;

		_D3DRENDERSTATETYPE type{};
		DWORD* value = nullptr;
		switch (var)
		{
		default:
			LOG_DBG("[CFogManager::Set] Invalid variable type[{}]!", static_cast<int>(var));
			return;
			break;

		case EFogVar::enable:
			type = D3DRS_FOGENABLE;
			value = reinterpret_cast<DWORD*>(&enable);
			break;

		case EFogVar::range_based:
			type = D3DRS_RANGEFOGENABLE;
			value = reinterpret_cast<DWORD*>(&range_based);
			break;

		case EFogVar::color:
			type = D3DRS_FOGCOLOR;
			value = reinterpret_cast<DWORD*>(&color);
			break;

		case EFogVar::mode:
			type = D3DRS_FOGTABLEMODE;
			if (mode >= 0 && mode <= 3)
				value = reinterpret_cast<DWORD*>(&mode);
			else
				value = reinterpret_cast<DWORD*>(&mode_max);
			break;

		case EFogVar::mode_vertex:
			type = D3DRS_FOGVERTEXMODE;
			if (mode_vertex >= 0 && mode_vertex <= 3)
				value = reinterpret_cast<DWORD*>(&mode_vertex);
			else
				value = reinterpret_cast<DWORD*>(&mode_max);
			break;

		case EFogVar::start:
			type = D3DRS_FOGSTART;
			value = reinterpret_cast<DWORD*>(&start);
			//orgSetRenderState(start, end, density);
			break;

		case EFogVar::end:
			type = D3DRS_FOGEND;
			value = reinterpret_cast<DWORD*>(&end);
			//orgSetRenderState(start, end, density);
			break;

		case EFogVar::density:
			type = D3DRS_FOGDENSITY;
			value = reinterpret_cast<DWORD*>(&density);
			//orgSetRenderState(start, end, density);
			break;
		}

		//SetInternal(type, value);
	}

	const bool IsReady() const { return m_bReady; }
};
