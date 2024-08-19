/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Window.hpp"
#include <Console.hpp>
#include <EDR.hpp>
#include "Tools.hpp"
#include "Mouse.hpp"
#include "Keyboard.hpp"
#include <stb_image.h>

#include "Layer.hpp"

bool eWindow::INIT()
{
	if (!glfwInit())
	{
		eConsole::PrintLog("GLFW ERROR", "glfwInit() returned false!");
		eConsole::WriteToDisk();
		return false;
	}
	return true;
}

void eWindow::UPDATE()
{
	glfwPollEvents();
}

void eWindow::RELEASE()
{
	glfwTerminate();
}

eWindow::eWindow()
{
	mWnd = nullptr;
}

eWindow::~eWindow()
{
	Unload();
	mSize = vec2i(0);
	mTitle.Clear();
}

bool eWindow::Initialize()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	mWnd = glfwCreateWindow(mSize.x, mSize.y, mTitle.Get(), nullptr, nullptr);

	glfwShowWindow(mWnd);

	mMouse = new eMouse();
	mMouse->SetWindow(this);

	mKeyboard = new eKeyboard();
	mKeyboard->SetWindow(this);
	return true;
}

void eWindow::Unload()
{
	if (mMouse)
		delete mMouse;

	if (mKeyboard)
		delete mKeyboard;

	glfwDestroyWindow(mWnd);
}

void eWindow::SetSize(vec2i size)
{
	mSize = size;
}

vec2i eWindow::GetSize()
{
	return mSize;
}

void eWindow::SetTitle(eString title)
{
	mTitle = title;
	if (mWnd != nullptr)
		glfwSetWindowTitle(mWnd, mTitle.Get());
}

eString eWindow::GetTitle()
{
	return mTitle;
}

int eWindow::ShowMessageBox(eString title, eString content, bool okaycancel)
{
	strg _content = content.GetStrg();
	strg _title = title.GetStrg();
	return MessageBox(NULL, std::wstring(_content.begin(), _content.end()).c_str(), std::wstring(_title.begin(), _title.end()).c_str(), okaycancel ? MB_OKCANCEL | MB_SYSTEMMODAL : NULL);
}

HWND eWindow::GetWnd()
{
	return glfwGetWin32Window(mWnd);
}

bool eWindow::WindowCloseRequested()
{
	return glfwWindowShouldClose(mWnd);
}

void eWindow::SetWindowCloseRequest(bool vl)
{
	glfwSetWindowShouldClose(mWnd, vl);
}

void eWindow::FocusWindow()
{
	glfwFocusWindow(mWnd);
}

bool eWindow::IsWindowVisible()
{
	return static_cast<bool>(glfwGetWindowAttrib(mWnd, GLFW_VISIBLE));
}

void eWindow::SetWindowVisibility(bool vl)
{
	vl ? glfwShowWindow(mWnd) : glfwHideWindow(mWnd);
}

void eWindow::IconifyWindow()
{
	glfwIconifyWindow(mWnd);
}

bool eWindow::IsWindowIconified()
{
	return static_cast<bool>(glfwGetWindowAttrib(mWnd, GLFW_ICONIFIED));
}

bool eWindow::IsWindowFocused()
{
	return static_cast<bool>(glfwGetWindowAttrib(mWnd, GLFW_FOCUSED));
}

void eWindow::SetWindowResizable(bool vl)
{
	glfwSetWindowAttrib(mWnd, GLFW_RESIZABLE, vl);
}

void eWindow::SetWindowDecorated(bool vl)
{
	glfwSetWindowAttrib(mWnd, GLFW_DECORATED, vl);
}

bool eWindow::IsWindowMaximized()
{
	return static_cast<bool>(glfwGetWindowAttrib(mWnd, GLFW_MAXIMIZED));
}

void eWindow::RestoreWindow()
{
	glfwRestoreWindow(mWnd);
}

void eWindow::MaximizeWindow()
{
	glfwMaximizeWindow(mWnd);
}

void eWindow::SetWindowAspectRatio(vec2i aspect)
{
	glfwSetWindowAspectRatio(mWnd, aspect.x, aspect.y);
}

void eWindow::SetWindowSizeLimits(vec2i min, vec2i max)
{
	int minX = min.x;
	int minY = min.y;
	int maxX = max.x;
	int maxY = max.y;
	if (min.x == 0 && min.y == 0)
		minX = minY = GLFW_DONT_CARE;
	if (max.x == 0 && max.y == 0)
		maxX = maxY = GLFW_DONT_CARE;
	glfwSetWindowSizeLimits(mWnd, minX, minY, maxX, maxY);
}

void eWindow::SetWindowIcon(eString filePath, real3 sx, real3 sy, int desiredChannels)
{
	const char* fp = filePath.Get();
	if (EDR::FileExists(fp))
	{
		GLFWimage* iconImg = new GLFWimage();
		iconImg->width = sx;
		iconImg->height = sy;
		iconImg->pixels = stbi_load(fp, &iconImg->width, &iconImg->height, NULL, desiredChannels);
		glfwSetWindowIcon(mWnd, 1, iconImg);
		stbi_image_free(iconImg->pixels);
		delete iconImg;
	}
	else
		eConsole::PrintLog("WARNING::eWindow::SetWindowIcon", "Icon file '" + filePath.GetStrg() + "' could not be found!");
}

void eWindow::CenterWindow()
{
	int w, h;
	glfwGetWindowSize(mWnd, &w, &h);

	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(mWnd, (vidmode->width / 2) - (w / 2), (vidmode->height / 2) - (h / 2));
}

vec2i eWindow::GetWindowSize()
{
	int w, h;
	glfwGetWindowSize(mWnd, &w, &h);
	return vec2i(w, h);
}

void eWindow::SetWindowPosition(vec2i pos)
{
	glfwSetWindowPos(mWnd, pos.x, pos.y);
}

vec2i eWindow::GetWindowPosition()
{
	int x, y;
	glfwGetWindowPos(mWnd, &x, &y);
	return vec2i(x, y);
}

void eWindow::SetWindowSize(vec2i size)
{
	glfwSetWindowSize(mWnd, size.x, size.y);
}

void eWindow::SwitchFullscreen()
{
	int w, h;
	glfwGetWindowSize(mWnd, &w, &h);

	GLFWmonitor* currentMonitor = get_current_monitor(mWnd);
	glfwSetWindowMonitor(mWnd, currentMonitor, 0, 0, w, h, GLFW_DONT_CARE);
}

void eWindow::AutoSize()
{
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int w = (uint)vidmode->width;
	int h = (uint)vidmode->height;
	glfwSetWindowSize(mWnd, w, h);
}

vec2i eWindow::GetMonitorSize()
{
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int w = (uint)vidmode->width;
	int h = (uint)vidmode->height;
	return vec2i(w, h);
}

void eWindow::SetUserPointer(void* ptr)
{
	glfwSetWindowUserPointer(mWnd, ptr);
}

void* eWindow::GetUserPointer()
{
	return glfwGetWindowUserPointer(mWnd);
}

GLFWwindow* eWindow::GetGLFWwindow()
{
	return mWnd;
}

eMouse* eWindow::GetMouse()
{
	return mMouse;
}

eKeyboard* eWindow::GetKeyboard()
{
	return mKeyboard;
}

GLFWmonitor* eWindow::get_current_monitor(GLFWwindow* window)
{
	int nmonitors, i;
	int wx, wy, ww, wh;
	int mx, my, mw, mh;
	int overlap, bestoverlap;
	GLFWmonitor* bestmonitor;
	GLFWmonitor** monitors;
	const GLFWvidmode* mode;

	bestoverlap = 0;
	bestmonitor = NULL;

	glfwGetWindowPos(window, &wx, &wy);
	glfwGetWindowSize(window, &ww, &wh);
	monitors = glfwGetMonitors(&nmonitors);

	for (i = 0; i < nmonitors; i++)
	{
		mode = glfwGetVideoMode(monitors[i]);
		glfwGetMonitorPos(monitors[i], &mx, &my);
		mw = mode->width;
		mh = mode->height;

		overlap = eTools::maxi(0, eTools::mini(wx + ww, mx + mw) - eTools::maxi(wx, mx)) * eTools::maxi(0, eTools::mini(wy + wh, my + mh) - eTools::maxi(wy, my));

		if (bestoverlap < overlap)
		{
			bestoverlap = overlap;
			bestmonitor = monitors[i];
		}
	}
	return bestmonitor;
}

void eWindow::SwitchFullscreenEx()
{
	SetWindowDecorated(false);
	SetWindowPosition(vec2i(0));
	AutoSize();
}

bool eWindow::SizeChanged()
{
	const vec2i asize = GetWindowSize();
	if (asize != mSize)
	{
		eLayer::mRefreshSize = true;
		mSize = asize;
		return true;
	}
	return false;
}