#pragma once

struct qevent_t
{
	int unk1;
	int unk2;
	int unk3;
	int unk4;
	int unk5;
	int unk6;
	int unk7;
};
static_assert(sizeof(qevent_t) == 0x1C, "Invalid dak_event_t size!"); // 28

namespace QEvents
{
	extern int GetCount();
	extern int GetTail();
	extern qevent_t* GetById(int id);
}