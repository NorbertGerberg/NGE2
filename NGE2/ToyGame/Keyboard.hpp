/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
#include <Types.hpp>
#include "Input.hpp"
#include <vector>

#define NGE_KEYBOARD_CHARCALLBACK GLFWcharfun

constexpr auto NGE_INPUT_KEY_UNKNOWN = -1;
constexpr auto NGE_INPUT_KEY_SPACE = 32;
constexpr auto NGE_INPUT_KEY_APOSTROPHE = 39;
constexpr auto NGE_INPUT_KEY_COMMA = 44;
constexpr auto NGE_INPUT_KEY_MINUS = 45;
constexpr auto NGE_INPUT_KEY_PERIOD = 46;
constexpr auto NGE_INPUT_KEY_SLASH = 47;
constexpr auto NGE_INPUT_KEY_0 = 48;
constexpr auto NGE_INPUT_KEY_1 = 49;
constexpr auto NGE_INPUT_KEY_2 = 50;
constexpr auto NGE_INPUT_KEY_3 = 51;
constexpr auto NGE_INPUT_KEY_4 = 52;
constexpr auto NGE_INPUT_KEY_5 = 53;
constexpr auto NGE_INPUT_KEY_6 = 54;
constexpr auto NGE_INPUT_KEY_7 = 55;
constexpr auto NGE_INPUT_KEY_8 = 56;
constexpr auto NGE_INPUT_KEY_9 = 57;
constexpr auto NGE_INPUT_KEY_SEMICOLON = 59;
constexpr auto NGE_INPUT_KEY_EQUAL = 61;
constexpr auto NGE_INPUT_KEY_A = 65;
constexpr auto NGE_INPUT_KEY_B = 66;
constexpr auto NGE_INPUT_KEY_C = 67;
constexpr auto NGE_INPUT_KEY_D = 68;
constexpr auto NGE_INPUT_KEY_E = 69;
constexpr auto NGE_INPUT_KEY_F = 70;
constexpr auto NGE_INPUT_KEY_G = 71;
constexpr auto NGE_INPUT_KEY_H = 72;
constexpr auto NGE_INPUT_KEY_I = 73;
constexpr auto NGE_INPUT_KEY_J = 74;
constexpr auto NGE_INPUT_KEY_K = 75;
constexpr auto NGE_INPUT_KEY_L = 76;
constexpr auto NGE_INPUT_KEY_M = 77;
constexpr auto NGE_INPUT_KEY_N = 78;
constexpr auto NGE_INPUT_KEY_O = 79;
constexpr auto NGE_INPUT_KEY_P = 80;
constexpr auto NGE_INPUT_KEY_Q = 81;
constexpr auto NGE_INPUT_KEY_R = 82;
constexpr auto NGE_INPUT_KEY_S = 83;
constexpr auto NGE_INPUT_KEY_T = 84;
constexpr auto NGE_INPUT_KEY_U = 85;
constexpr auto NGE_INPUT_KEY_V = 86;
constexpr auto NGE_INPUT_KEY_W = 87;
constexpr auto NGE_INPUT_KEY_X = 88;
constexpr auto NGE_INPUT_KEY_Y = 90; //German keyboard layout
constexpr auto NGE_INPUT_KEY_Z = 89;
constexpr auto NGE_INPUT_KEY_LEFT_BRACKET = 91;
constexpr auto NGE_INPUT_KEY_BACKSLASH = 92;
constexpr auto NGE_INPUT_KEY_RIGHT_BRACKET = 93;
constexpr auto NGE_INPUT_KEY_GRAVE_ACCENT = 96;
constexpr auto NGE_INPUT_KEY_WORLD_1 = 161;
constexpr auto NGE_INPUT_KEY_WORLD_2 = 162;
constexpr auto NGE_INPUT_KEY_ESCAPE = 256;
constexpr auto NGE_INPUT_KEY_ENTER = 257;
constexpr auto NGE_INPUT_KEY_TAB = 258;
constexpr auto NGE_INPUT_KEY_BACKSPACE = 259;
constexpr auto NGE_INPUT_KEY_INSERT = 260;
constexpr auto NGE_INPUT_KEY_DELETE = 261;
constexpr auto NGE_INPUT_KEY_RIGHT = 262;
constexpr auto NGE_INPUT_KEY_LEFT = 263;
constexpr auto NGE_INPUT_KEY_DOWN = 264;
constexpr auto NGE_INPUT_KEY_UP = 265;
constexpr auto NGE_INPUT_KEY_PAGE_UP = 266;
constexpr auto NGE_INPUT_KEY_PAGE_DOWN = 267;
constexpr auto NGE_INPUT_KEY_HOME = 268;
constexpr auto NGE_INPUT_KEY_END = 269;
constexpr auto NGE_INPUT_KEY_CAPS_LOCK = 280;
constexpr auto NGE_INPUT_KEY_SCROLL_LOCK = 281;
constexpr auto NGE_INPUT_KEY_NUM_LOCK = 282;
constexpr auto NGE_INPUT_KEY_PRINT_SCREEN = 283;
constexpr auto NGE_INPUT_KEY_PAUSE = 284;
constexpr auto NGE_INPUT_KEY_F1 = 290;
constexpr auto NGE_INPUT_KEY_F2 = 291;
constexpr auto NGE_INPUT_KEY_F3 = 292;
constexpr auto NGE_INPUT_KEY_F4 = 293;
constexpr auto NGE_INPUT_KEY_F5 = 294;
constexpr auto NGE_INPUT_KEY_F6 = 295;
constexpr auto NGE_INPUT_KEY_F7 = 296;
constexpr auto NGE_INPUT_KEY_F8 = 297;
constexpr auto NGE_INPUT_KEY_F9 = 298;
constexpr auto NGE_INPUT_KEY_F10 = 299;
constexpr auto NGE_INPUT_KEY_F11 = 300;
constexpr auto NGE_INPUT_KEY_F12 = 301;
constexpr auto NGE_INPUT_KEY_F13 = 302;
constexpr auto NGE_INPUT_KEY_F14 = 303;
constexpr auto NGE_INPUT_KEY_F15 = 304;
constexpr auto NGE_INPUT_KEY_F16 = 305;
constexpr auto NGE_INPUT_KEY_F17 = 306;
constexpr auto NGE_INPUT_KEY_F18 = 307;
constexpr auto NGE_INPUT_KEY_F19 = 308;
constexpr auto NGE_INPUT_KEY_F20 = 309;
constexpr auto NGE_INPUT_KEY_F21 = 310;
constexpr auto NGE_INPUT_KEY_F22 = 311;
constexpr auto NGE_INPUT_KEY_F23 = 312;
constexpr auto NGE_INPUT_KEY_F24 = 313;
constexpr auto NGE_INPUT_KEY_F25 = 314;
constexpr auto NGE_INPUT_KEY_KP_0 = 320;
constexpr auto NGE_INPUT_KEY_KP_1 = 321;
constexpr auto NGE_INPUT_KEY_KP_2 = 322;
constexpr auto NGE_INPUT_KEY_KP_3 = 323;
constexpr auto NGE_INPUT_KEY_KP_4 = 324;
constexpr auto NGE_INPUT_KEY_KP_5 = 325;
constexpr auto NGE_INPUT_KEY_KP_6 = 326;
constexpr auto NGE_INPUT_KEY_KP_7 = 327;
constexpr auto NGE_INPUT_KEY_KP_8 = 328;
constexpr auto NGE_INPUT_KEY_KP_9 = 329;
constexpr auto NGE_INPUT_KEY_KP_DECIMAL = 330;
constexpr auto NGE_INPUT_KEY_KP_DIVIDE = 331;
constexpr auto NGE_INPUT_KEY_KP_MULTIPLY = 332;
constexpr auto NGE_INPUT_KEY_KP_SUBTRACT = 333;
constexpr auto NGE_INPUT_KEY_KP_ADD = 334;
constexpr auto NGE_INPUT_KEY_KP_ENTER = 335;
constexpr auto NGE_INPUT_KEY_KP_EQUAL = 336;
constexpr auto NGE_INPUT_KEY_LEFT_SHIFT = 340;
constexpr auto NGE_INPUT_KEY_LEFT_CONTROL = 341;
constexpr auto NGE_INPUT_KEY_LEFT_ALT = 342;
constexpr auto NGE_INPUT_KEY_LEFT_SUPER = 343;
constexpr auto NGE_INPUT_KEY_RIGHT_SHIFT = 344;
constexpr auto NGE_INPUT_KEY_RIGHT_CONTROL = 345;
constexpr auto NGE_INPUT_KEY_RIGHT_ALT = 346;
constexpr auto NGE_INPUT_KEY_RIGHT_SUPER = 347;
constexpr auto NGE_INPUT_KEY_MENU = 348;
constexpr auto NGE_INPUT_KEY_LAST = 348;
constexpr auto NGE_INPUT_MOD_SHIFT = 0x0001;
constexpr auto NGE_INPUT_MOD_CONTROL = 0x0002;
constexpr auto NGE_INPUT_MOD_ALT = 0x0004;
constexpr auto NGE_INPUT_MOD_SUPER = 0x0008;
constexpr auto NGE_INPUT_MOD_CAPS_LOCK = 0x0010;
constexpr auto NGE_INPUT_MOD_NUM_LOCK = 0x0020;

class eKeyboard : public eInput
{
public:
	//Physical input
	const bool GetKeyPressed(const int key);
	const bool GetKeyDown(const int key);
	const bool GetKeyReleased(const int key);
	const int GetScancode(const int key);
	const int GetKeyState(const int key);
	const bool CheckKeyState(const int keyState, const int mode);
	void SetStickyMode(const int vl);

	//Text input
	void SetCharCallback(NGE_KEYBOARD_CHARCALLBACK& callback);
	const char* GetKeyName(const int key, const int scancode);

private:
	std::vector<int> mKeyPressed;
};
#endif