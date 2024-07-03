/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <Types.hpp>
#include <String.hpp>
#include "Time.hpp"

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Windows.h>

class eMouse;
class eKeyboard;

class eWindow
{
public:
	static bool INIT();
	static void UPDATE();
	static void RELEASE();

	eWindow();
	~eWindow();

	bool Initialize();
	void Unload();

	void SetSize(vec2i size);
	vec2i GetSize();

	void SetTitle(eString title);
	eString GetTitle();

	int ShowMessageBox(eString title, eString content, bool okaycancel);

	HWND GetWnd();
	bool WindowCloseRequested();
	void SetWindowCloseRequest(bool vl);
	void FocusWindow();
	bool IsWindowFocused();
	bool IsWindowVisible();
	void SetWindowVisibility(bool vl);
	void IconifyWindow();
	bool IsWindowIconified();
	void SetWindowResizable(bool vl);
	void SetWindowDecorated(bool vl);
	bool IsWindowMaximized();
	void RestoreWindow();
	void MaximizeWindow();
	void SetWindowAspectRatio(vec2i aspect);
	void SetWindowSizeLimits(vec2i min, vec2i max);
	void SetWindowIcon(eString filePath, real3 sx, real3 sy, int desiredChannels);
	void CenterWindow();
	vec2i GetWindowSize();
	void SetWindowPosition(vec2i pos);
	vec2i GetWindowPosition();
	void SetWindowSize(vec2i size);
	void SwitchFullscreen();
	void AutoSize();
	vec2i GetMonitorSize();
	void SetUserPointer(void* ptr);
	void* GetUserPointer();
	void SwitchFullscreenEx();

	GLFWwindow* GetGLFWwindow();

	eMouse* GetMouse();
	eKeyboard* GetKeyboard();

	bool SizeChanged();

private:
	GLFWwindow* mWnd;

	vec2i mSize;
	eString mTitle;

	eMouse* mMouse;
	eKeyboard* mKeyboard;

	GLFWmonitor* get_current_monitor(GLFWwindow* window);
};
#endif