#include "precompiled.hpp"

extern renderer_data_t* g_pGameRenderer;

static IDirect3DBaseTexture9* g_pLastTex = NULL;
static bool g_bFvfSet = false;

using SetTexture_T = void(__fastcall*)(IDirect3DTexture9** tex, int flipMode, int flipFlag, int vertexCount, void* vertexData);
static SetTexture_T pfnSetTexture;
static void __fastcall SetTexture(IDirect3DTexture9** tex, int flipMode, int flipFlag, int vertexCount, void* vertexData)
{
	if (!pfnSetTexture)
		return;

	if (!tex || !*tex) return;

	// Кэшировать SetTexture — вызывать только при смене текстуры
	if (*tex != g_pLastTex) {
		g_pGameRenderer->pD3d->pDevice->SetTexture(0, *tex);
		g_pLastTex = *tex;
	}

	// SetFVF — всегда 452, вызвать один раз
	if (!g_bFvfSet) {
		g_pGameRenderer->pD3d->pDevice->SetFVF(452);
		g_bFvfSet = true;
	}

	// Оригинальная трансформация вершин + DrawPrimitiveUP
	pfnSetTexture(tex, flipMode, flipFlag, vertexCount, vertexData);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ?? ?? ?? ?? 83 3D ?? ?? ?? ?? ?? 74 2B", pfnSetTexture, SetTexture);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);