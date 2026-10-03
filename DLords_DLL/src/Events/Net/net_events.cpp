#include "precompiled.hpp"

HandleNetPacket_t pfnHandleNetPacket;

static void __fastcall HandleNetPacket(int senderId, EPacketType packetType, void* data)
{
	LOG("[HandleNetPacket] Sender[{}] packetType[{}].", senderId, PacketNumToStr(packetType));

	pfnHandleNetPacket(senderId, packetType, data);
}

static void Init(HookChain_Init::ICallback* chain, CHookChainArgs_Init& args)
{
	CREATE_GEH("E8 ? ? ? ? E8 ? ? ? ? 84 C0 0F 85", pfnHandleNetPacket, HandleNetPacket);

	chain->callNext(args);
}

REGISTER_CALLBACK(Init, Init);