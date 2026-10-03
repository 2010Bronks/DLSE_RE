#include "precompiled.hpp"

/*
	If you want to play the game in windowed mode, you must use the -gdi
	startup parameter. However, this will break character camera control.
	You won't be able to rotate the camera properly until you open any interface.

	This fix hooks the game's UpdateCursorLock function and modifies the
	newLockX/newLockY values by resetting the cursor position to the screen center.

	The s_pShowCursor check is used to prevent modifying the cursor position
	while a user interface is shown.
*/

bool gbFixWindowedCursor = true;

using UpdateCursorLock_t = void(__fastcall*)(int newLockX, int newLockY, int bSet);
static UpdateCursorLock_t pfnUpdateCursorLock = nullptr;
static BOOL* s_pShowCursor = nullptr;

static bool ProcessFix(int& newLockX, int& newLockY, int& bSet)
{
	if (!g_pGameRenderer || !g_pGameRenderer->IsReady())
		return false;

	if (!s_pShowCursor)
		return false;

	if ((*s_pShowCursor) != 0)
		return false;

	if (newLockX > (g_pGameRenderer->sCurDisplayMode.iWidth - 20) || newLockX < 20)
	{
		newLockX = g_pGameRenderer->sCurDisplayMode.iWidth / 2;
		bSet = 1;
	}
	
	if (newLockY > (g_pGameRenderer->sCurDisplayMode.iHeight - 20) || newLockY < 20)
	{
		newLockY = g_pGameRenderer->sCurDisplayMode.iHeight / 2;
		bSet = 1;
	}

	return (bSet == 1);
}

static void __fastcall UpdateCursorLock(int newLockX, int newLockY, int bSet)
{
	if (!pfnUpdateCursorLock)
		return;

	if (gbFixWindowedCursor)
	{
		ProcessFix(newLockX, newLockY, bSet);
	}

	pfnUpdateCursorLock(newLockX, newLockY, bSet);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GES("74 0B 89 0D ?? ?? ?? ?? E9 8D FE FF FF", s_pShowCursor, 4);
	CREATE_GEH("E8 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 85 C0", pfnUpdateCursorLock, UpdateCursorLock);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);