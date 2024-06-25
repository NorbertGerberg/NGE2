/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Keyboard.hpp"

const bool eKeyboard::GetKeyPressed(const int key)
{
	bool isDown		= false;
	int location	= 0;
	for (int i = 0; i < mKeyPressed.size(); i++)
	{
		if (mKeyPressed[i] == key)
		{
			location = i;
			isDown = true;
			break;
		}
	}

	if (isDown)
	{
		if (GetKeyReleased(key))
			mKeyPressed.erase(mKeyPressed.begin() + location);
		return false;
	}
	else
	{
		if (GetKeyDown(key))
		{
			mKeyPressed.push_back(key);
			return true;
		}
	}
	return false;
}

const bool eKeyboard::GetKeyDown(const int key)
{
	if (glfwGetKey(GetGLFWwindow(), key) == GLFW_PRESS)
		return true;
	return false;
}

const bool eKeyboard::GetKeyReleased(const int key)
{
	if (glfwGetKey(GetGLFWwindow(), key) == GLFW_RELEASE)
		return true;
	return false;
}

const int eKeyboard::GetScancode(const int key)
{
	return glfwGetKeyScancode(key);
}

const int eKeyboard::GetKeyState(const int key)
{
	return glfwGetKey(GetGLFWwindow(), key);
}

const bool eKeyboard::CheckKeyState(const int keyState, const int mode)
{
	if (keyState == mode)
		return true;
	return false;
}

void eKeyboard::SetStickyMode(const int vl)
{
	glfwSetInputMode(GetGLFWwindow(), GLFW_STICKY_KEYS, vl);
}

void eKeyboard::SetCharCallback(NGE_KEYBOARD_CHARCALLBACK& callback)
{
	glfwSetCharCallback(GetGLFWwindow(), callback);
}

const char* eKeyboard::GetKeyName(const int key, const int scancode)
{
	return glfwGetKeyName(key, scancode);
}