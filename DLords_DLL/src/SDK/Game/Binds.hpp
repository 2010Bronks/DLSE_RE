#pragma once

struct keybind_t
{
	keybind_t* pNext;
	unsigned char key;
	bool unk1;
	bool isPlusCommand;
	char command[128];
}; 
static_assert(sizeof(keybind_t) == 136, "Invalid keybind_t size");

using CommandCb_t = void(*)();
struct command_t
{
	char name[32];
	CommandCb_t callback1;
	CommandCb_t callback2;
};
static_assert(sizeof(command_t) == 40, "Invalid command_t size");

extern const std::unordered_map<unsigned char, std::string>& GetKeys();
extern bool IsKeyRegistered(unsigned char key);
extern std::string GetKeyName(unsigned char key);