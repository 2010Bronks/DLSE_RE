#pragma once

using HandleNetPacket_t = void(__fastcall*)(int senderId, EPacketType packetType, void* data);