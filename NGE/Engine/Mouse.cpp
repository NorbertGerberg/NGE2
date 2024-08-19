/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Mouse.hpp"
#include <Console.hpp>
#include "Texture.hpp"
#include <stb_image.h>

void eMouse::SetPositionCallback(NGE_MOUSE_POSCALLBACK callback)
{
	glfwSetCursorPosCallback(GetGLFWwindow(), callback);
}

void eMouse::GetPosition(real6& xpos, real6& ypos)
{
	glfwGetCursorPos(GetGLFWwindow(), &xpos, &ypos);
}

void eMouse::DisableCursor()
{
	glfwSetInputMode(GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void eMouse::HideCursor()
{
	glfwSetInputMode(GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void eMouse::NormalCursor()
{
	glfwSetInputMode(GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void eMouse::CheckRawMouseMotionSupport()
{
	if (glfwRawMouseMotionSupported()) {
		eConsole::PrintLog("INFO::eMouse::CheckRawMouseMotionSupport", "Current machine does support raw mouse motion");
		glfwSetInputMode(GetGLFWwindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		return;
	}
	eConsole::PrintLog("INFO::eMouse::CheckRawMouseMotionSupport", "Current machine does not support raw mouse motion");
	glfwSetInputMode(GetGLFWwindow(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
}

void eMouse::SetEnterCallback(NGE_MOUSE_ENTERCALLBACK callback)
{
	glfwSetCursorEnterCallback(GetGLFWwindow(), callback);
}

const bool eMouse::IsHovering()
{
	return glfwGetWindowAttrib(GetGLFWwindow(), GLFW_HOVERED);
}

void eMouse::SetMouseButtonCallback(NGE_MOUSE_BUTTONCALLBACK callback)
{
	glfwSetMouseButtonCallback(GetGLFWwindow(), callback);
}

const int eMouse::GetButtonState(const int button)
{
	return glfwGetMouseButton(GetGLFWwindow(), button);
}

void eMouse::SetStickyMode(const int vl)
{
	glfwSetInputMode(GetGLFWwindow(), GLFW_STICKY_MOUSE_BUTTONS, vl);
}

void eMouse::SetScrollCallback(NGE_MOUSE_SCROLLCALLBACK callback)
{
	glfwSetScrollCallback(GetGLFWwindow(), callback);
}

const bool eMouse::GetButtonPressed(const int button)
{
	bool isDown = false;
	int location = 0;
	for (int i = 0; i < mButtonPressed.size(); i++)
	{
		if (mButtonPressed[i] == button)
		{
			location = i;
			isDown = true;
			break;
		}
	}

	if (isDown)
	{
		if (GetButtonReleased(button))
			mButtonPressed.erase(mButtonPressed.begin() + location);
		return false;
	}
	else
	{
		if (GetButtonDown(button))
		{
			mButtonPressed.push_back(button);
			return true;
		}
	}
	return false;
}

const bool eMouse::GetButtonDown(const int button)
{
	if (glfwGetMouseButton(GetGLFWwindow(), button) == GLFW_PRESS)
		return true;
	return false;
}

const bool eMouse::GetButtonReleased(const int button)
{
	if (glfwGetMouseButton(GetGLFWwindow(), button) == GLFW_RELEASE)
		return true;
	return false;
}

void eMouse::CreateCursor(eMouseCursor* cursor, int type)
{
	if (cursor != nullptr)
		glfwDestroyCursor(cursor);
	cursor = glfwCreateStandardCursor(type);
}

eMouseCursor* eMouse::CreateImageCursor(eMouseCursor* cursor, eString filename, bool flipV, vec2i xyhot)
{
	if (cursor != nullptr)
		glfwDestroyCursor(cursor);

	stbi_set_flip_vertically_on_load(flipV);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.Get(), &width, &height, &nrComponents, 0);
	if (data != nullptr)
	{
		GLFWimage image;
		image.width = width;
		image.height = height;
		image.pixels = data;
		cursor = glfwCreateCursor(&image, xyhot.x, xyhot.y);
		stbi_image_free(data);
	}
	stbi_set_flip_vertically_on_load(false);
	return cursor;
}

void eMouse::SetCursor(eMouseCursor* cursor)
{
	glfwSetCursor(GetGLFWwindow(), cursor);
}

eMouseCursor* eMouse::DestroyCursor(eMouseCursor* cursor)
{
	if (cursor != nullptr)
	{
		glfwDestroyCursor(cursor);
		cursor = nullptr;
	}
	return nullptr;
}