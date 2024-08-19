/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MOUSE_HPP
#define MOUSE_HPP
#include <Types.hpp>
#include "Input.hpp"
#include <vector>

#define NGE_MOUSE_POSCALLBACK	 GLFWcursorposfun
#define NGE_MOUSE_ENTERCALLBACK	 GLFWcursorenterfun
#define NGE_MOUSE_BUTTONCALLBACK GLFWmousebuttonfun
#define NGE_MOUSE_SCROLLCALLBACK GLFWscrollfun

constexpr auto NGE_INPUT_CURSOR = 0x00033001;
constexpr auto NGE_INPUT_STICKY_MOUSE_BUTTONS = 0x00033003;
constexpr auto NGE_INPUT_RAW_MOUSE_MOTION = 0x00033005;
constexpr auto NGE_INPUT_CURSOR_NORMAL = 0x00034001;
constexpr auto NGE_INPUT_CURSOR_HIDDEN = 0x00034002;
constexpr auto NGE_INPUT_CURSOR_DISABLED = 0x00034003;

constexpr auto NGE_INPUT_MOUSE_BUTTON_1 = 0;
constexpr auto NGE_INPUT_MOUSE_BUTTON_2 = 1;
constexpr auto NGE_INPUT_MOUSE_BUTTON_3 = 2;
constexpr auto NGE_INPUT_MOUSE_BUTTON_4 = 3;
constexpr auto NGE_INPUT_MOUSE_BUTTON_5 = 4;
constexpr auto NGE_INPUT_MOUSE_BUTTON_6 = 5;
constexpr auto NGE_INPUT_MOUSE_BUTTON_7 = 6;
constexpr auto NGE_INPUT_MOUSE_BUTTON_8 = 7;
constexpr auto NGE_INPUT_MOUSE_BUTTON_LAST = 7;
constexpr auto NGE_INPUT_MOUSE_BUTTON_LEFT = 0;
constexpr auto NGE_INPUT_MOUSE_BUTTON_RIGHT = 1;
constexpr auto NGE_INPUT_MOUSE_BUTTON_MIDDLE = 2;

constexpr auto NGE_INPUT_MOUSE_CURSOR_ARROW			= 0x00036001;
constexpr auto NGE_INPUT_MOUSE_CURSOR_IBEAM			= 0x00036002;
constexpr auto NGE_INPUT_MOUSE_CURSOR_CROSSHAIR		= 0x00036003;
constexpr auto NGE_INPUT_MOUSE_CURSOR_HAND			= 0x00036004;
constexpr auto NGE_INPUT_MOUSE_CURSOR_HRESIZE		= 0x00036005;
constexpr auto NGE_INPUT_MOUSE_CURSOR_VRESIZE		= 0x00036006;

typedef GLFWcursor eMouseCursor;

class eTexture;

//! Mouse input
//! \ingroup Engine
class eMouse : public eInput
{
public:
	void SetPositionCallback(NGE_MOUSE_POSCALLBACK callback);
	void GetPosition(real6& xpos, real6& ypos);
	void DisableCursor();
	void HideCursor();
	void NormalCursor();
	void CheckRawMouseMotionSupport();
	void SetEnterCallback(NGE_MOUSE_ENTERCALLBACK callback);
	const bool IsHovering();
	void SetMouseButtonCallback(NGE_MOUSE_BUTTONCALLBACK callback);
	const int GetButtonState(const int button);
	void SetStickyMode(const int vl);
	void SetScrollCallback(NGE_MOUSE_SCROLLCALLBACK callback);
	const bool GetButtonPressed(const int button);
	const bool GetButtonDown(const int button);
	const bool GetButtonReleased(const int button);
	void CreateCursor(eMouseCursor* cursor, int type);
	eMouseCursor* CreateImageCursor(eMouseCursor* cursor, eString filename, bool flipV = false, vec2i xyhot = vec2i(0));
	void SetCursor(eMouseCursor* cursor);
	eMouseCursor* DestroyCursor(eMouseCursor* cursor);

private:
	std::vector<int> mButtonPressed;
};
#endif