#include "precompiled.hpp"

const std::unordered_map<unsigned char, std::string>& GetKeys()
{
	static std::unordered_map<unsigned char, std::string> Keys =
	{
		{ VK_LBUTTON, "Mouse 1" },
		{ VK_RBUTTON, "Mouse 2" },

		{ VK_MBUTTON, "Mouse 3" },
		{ VK_XBUTTON1, "Mouse 4" },
		{ VK_XBUTTON2, "Mouse 5" },
		{ 0x07, "Mouse 6" },

		{ VK_BACK, "Backspace" },
		{ VK_TAB, "Tab" },

		{ VK_CLEAR, "Clear" },
		{ VK_RETURN, "Enter" },

		{ VK_LSHIFT, "L Shift" },
		{ VK_RSHIFT, "R Shift" },
		{ VK_LCONTROL, "L Ctrl" },
		{ VK_RCONTROL, "R Ctrl" },
		{ VK_LMENU, "L Alt" },
		{ VK_RMENU, "R Alt" },

		{ VK_PAUSE, "Pause" },
		{ VK_CAPITAL, "Caps" },

		{ VK_ESCAPE, "Escape" },

		{ VK_SPACE,		"Space" },
		{ VK_PRIOR,		"Page Up" },
		{ VK_NEXT,		"Page Down" },
		{ VK_END,		"End" },
		{ VK_HOME,		"Home" },
		{ VK_LEFT,		"Left" },
		{ VK_UP,		"Up" },
		{ VK_RIGHT,		"Right" },
		{ VK_DOWN,		"Down" },
		{ VK_SELECT,	"Select" },
		{ VK_PRINT,		"Print" },
		{ VK_EXECUTE,	"Execute" },
		{ VK_SNAPSHOT,	"Prt Scrn" },
		{ VK_INSERT,	"Insert" },
		{ VK_DELETE,	"Delete" },
		{ VK_HELP,		"Help" },

		{ 'A', "A" },
		{ 'B', "B" },
		{ 'C', "C" },
		{ 'D', "D" },
		{ 'E', "E" },
		{ 'F', "F" },
		{ 'G', "G" },
		{ 'H', "H" },
		{ 'I', "I" },
		{ 'J', "J" },
		{ 'K', "K" },
		{ 'L', "L" },
		{ 'M', "M" },
		{ 'N', "N" },
		{ 'O', "O" },
		{ 'P', "P" },
		{ 'Q', "Q" },
		{ 'R', "R" },
		{ 'S', "S" },
		{ 'T', "T" },
		{ 'U', "U" },
		{ 'V', "V" },
		{ 'W', "W" },
		{ 'X', "X" },
		{ 'Y', "Y" },
		{ 'Z', "Z" },

		{ '0', "0" },
		{ '1', "1" },
		{ '2', "2" },
		{ '3', "3" },
		{ '4', "4" },
		{ '5', "5" },
		{ '6', "6" },
		{ '7', "7" },
		{ '8', "8" },
		{ '9', "9" },

		{ 222, "Quote" },
		{ 188, "Comma" },
		{ 189, "Hyphen" },
		{ 187, "Equals" },
		{ 190, "Period" },
		{ 191, "Slash" },
		{ 186, "Semicolon" },
		{ 219, "L Bracket" },
		{ 221, "R Bracket" },
		{ 220, "Backslash" },
		{ 192, "Grave" },

		{ VK_LWIN, "L Windows" },
		{ VK_RWIN, "R Windows" },
		{ VK_APPS, "Apps" },

		{ VK_SLEEP, "Sleep" },

		{ VK_NUMPAD0,   "Numpad 0" },
		{ VK_NUMPAD1,   "Numpad 1" },
		{ VK_NUMPAD2,   "Numpad 2" },
		{ VK_NUMPAD3,   "Numpad 3" },
		{ VK_NUMPAD4,   "Numpad 4" },
		{ VK_NUMPAD5,   "Numpad 5" },
		{ VK_NUMPAD6,   "Numpad 6" },
		{ VK_NUMPAD7,   "Numpad 7" },
		{ VK_NUMPAD8,   "Numpad 8" },
		{ VK_NUMPAD9,   "Numpad 9" },
		{ VK_MULTIPLY,  "Multiply" },
		{ VK_ADD,       "Add"},
		{ VK_SEPARATOR, "Separator"},
		{ VK_SUBTRACT,  "Subtract"},
		{ VK_DECIMAL,   "Decimal"},
		{ VK_DIVIDE,    "Divide"},

		{ VK_F1,  "F1"},
		{ VK_F2,  "F2"},
		{ VK_F3,  "F3"},
		{ VK_F4,  "F4"},
		{ VK_F5,  "F5"},
		{ VK_F6,  "F6"},
		{ VK_F7,  "F7"},
		{ VK_F8,  "F8"},
		{ VK_F9,  "F9"},
		{ VK_F10, "F10"},
		{ VK_F11, "F11"},
		{ VK_F12, "F12"},
		{ VK_F13, "F13"},
		{ VK_F14, "F14"},
		{ VK_F15, "F15"},
		{ VK_F16, "F16"},
		{ VK_F17, "F17"},
		{ VK_F18, "F18"},
		{ VK_F19, "F19"},
		{ VK_F20, "F20"},
		{ VK_F21, "F21"},
		{ VK_F22, "F22"},
		{ VK_F23, "F23"},
		{ VK_F24, "F24"},

		{ VK_KANA,		"Kana" },
		{ VK_HANGUL,	"Hangul" },
		{ VK_HANJA,		"Hanja" },
		{ VK_KANJI,		"Kanji" },
		{ VK_IME_ON,	"IME On" },
		{ VK_IME_OFF,	"IME Off" },
		{ VK_JUNJA,		"Junja" },
		{ VK_FINAL,		"Final" },
	};

	return Keys;
}

bool IsKeyRegistered(unsigned char key)
{
	if (key == 0)
		return false;

	return (GetKeys().find(key) != GetKeys().end());
}

std::string GetKeyName(unsigned char key)
{
	if (key == 0)
		return "0";

	if (auto it = GetKeys().find(key); it != GetKeys().end())
		return it->second.c_str();

	return std::to_string(key);
}