#include "precompiled.hpp"

static void* s_pCode = nullptr;
bool gbGravityDisabled = false;
float s_fGravity = 0.0f;

bool gbFlyMode = false;

static void EveryFrame()
{
	static bool wasDisabled = true;

	if (!gbFlyMode)
	{
		if (!wasDisabled && gbGravityDisabled)
		{
			gbGravityDisabled = false;
			wasDisabled = true;
		}

		return;
	}

	if (!gbGravityDisabled)
	{
		gbGravityDisabled = true;
		wasDisabled = false;
	}

	auto pLocal = GetLocalPlayer();
	if (!pLocal || !pLocal->GetData())
		return;

	auto pInv = &pLocal->GetData()->inventory;
	if (!pInv)
		return;
}

static void OnGameFrame(HookChain_OnGameFrame::ICallback* chain, CHookChainArgs_OnGameFrame& args)
{
	EveryFrame();

	chain->callNext(args);
}

static void __declspec(naked) PatchGravity()
{
	__asm
	{
		cmp byte ptr[gbGravityDisabled], 0
		je OriginalCode

		mov eax, dword ptr[s_fGravity]
		mov dword ptr[edx + 0B8h], eax

		OriginalCode :
		fcomp   dword ptr[edx + 0B8h]
			ret
	}
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	s_pCode = U::Memory::FindSignature(
		gGameModule->GetBase(), gGameModule->GetBase(), gGameModule->GetLastByte(),
		"D8 9A B8 00 00 00 DF E0 F6 C4 44 7A 0C D9 05 ?? ?? ?? ?? D9 9A B8 00 00 00 0F B7 81 E8 05 00 00"
	);

	if (s_pCode)
	{
		U::Memory::CreateMemoryBackup(s_pCode, 6);
		U::Memory::FillNops(s_pCode, 6);
		U::Memory::WriteCall(s_pCode, PatchGravity);
	}

	chain->callNext(args);
}

REGISTER_CALLBACK(OnGameFrame, OnGameFrame);
REGISTER_CALLBACK(Init, Init);