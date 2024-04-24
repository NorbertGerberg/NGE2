/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef INPUT_HPP
#define INPUT_HPP
#include <Types.hpp>
#include "Window.hpp"

constexpr auto NGE_INPUT_RELEASE = 0;
constexpr auto NGE_INPUT_PRESS = 1;
constexpr auto NGE_INPUT_REPEAT = 2;
constexpr auto NGE_INPUT_FALSE = 0;
constexpr auto NGE_INPUT_TRUE = 1;
constexpr auto NGE_INPUT_STICKY_KEYS = 0x00033002;
constexpr auto NGE_INPUT_LOCK_KEY_MODS = 0x00033004;

class eInput
{
public:
	void SetWindow(eWindow* window);
	eWindow* GetWindow();
	GLFWwindow* GetGLFWwindow();
	void SetInputMode(const int mode, const int value);

private:
	eWindow* mWindow;
};
#endif