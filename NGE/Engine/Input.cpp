/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Input.hpp"

void eInput::SetWindow(eWindow* window)
{
	mWindow = nullptr;
	mWindow = window;
}

eWindow* eInput::GetWindow()
{
	return mWindow;
}

GLFWwindow* eInput::GetGLFWwindow()
{
	return mWindow->GetGLFWwindow();
}

void eInput::SetInputMode(const int mode, const int value)
{
	glfwSetInputMode(mWindow->GetGLFWwindow(), mode, value);
}