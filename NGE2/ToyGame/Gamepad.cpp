/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Gamepad.hpp"
#include <EDR.hpp>
#include <Console.hpp>
#include <algorithm>

bool eGamepad::IsConnected(ulong port)
{
	ulong result = XInputGetState(port, &mState);
	if (result == ERROR_SUCCESS)
		return true;
	return false;
}

eDeadzoneStick eGamepad::DeadzoneCorrection()
{
	real6 LX = mState.Gamepad.sThumbLX;
	real6 LY = mState.Gamepad.sThumbLY;

	real6 magnitude = sqrt(LX * LX + LY * LY);

	real6 normalizedLX = LX / magnitude;
	real6 normalizedLY = LY / magnitude;
	real6 normalizedMagnitude = 0;

	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		if (magnitude > 32767) magnitude = 32767;
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else
	{
		magnitude = 0.0;
		normalizedMagnitude = 0.0;
	}

	real6 RX = mState.Gamepad.sThumbRX;
	real6 RY = mState.Gamepad.sThumbRY;

	real6 Rmagnitude = sqrt(RX * RX + RY * RY);

	real6 normalizedRX = RX / Rmagnitude;
	real6 normalizedRY = RY / Rmagnitude;
	real6 RnormalizedMagnitude = 0;

	if (Rmagnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		if (Rmagnitude > 32767) Rmagnitude = 32767;
		Rmagnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		RnormalizedMagnitude = Rmagnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	else
	{
		Rmagnitude = 0.0;
		RnormalizedMagnitude = 0.0;
	}

	//return { LX / normalizedMagnitude, LY / normalizedMagnitude, RX / RnormalizedMagnitude, RY / RnormalizedMagnitude };
	return { LX / magnitude, LY / magnitude, RX / Rmagnitude, RY / Rmagnitude };
}

void eGamepad::Vibration(ulong port, int leftmotorspeed, int rightmotorspeed)
{
	mVibration.wLeftMotorSpeed = leftmotorspeed;
	mVibration.wRightMotorSpeed = rightmotorspeed;
	XInputSetState(port, &mVibration);
}

bool eGamepad::GetButtonPressed(uint button)
{
	return (mState.Gamepad.wButtons & button);
}

bool eGamepad::GetButtonReleased(uint button)
{
	return !(mState.Gamepad.wButtons & button);
}

bool eGamepad::CheckAxis(real3 DeadzoneStick, real3 axis)
{
	const real3& _Axis = std::clamp(DeadzoneStick, -5.0f, 5.0f);
	real3 Axis;
	if (_Axis <= 1.3f && _Axis >= 0.0f)
		Axis = 5.0f;
	else if (_Axis >= -1.3f && _Axis <= 0.0f)
		Axis = -5.0f;
	else
		Axis = _Axis;

	if (axis <= 0.0f)
	{
		if (Axis >= axis && Axis <= 0.0f)
			return true;
		return false;
	}
	else if (axis >= 0.0f)
	{
		if (Axis <= axis && Axis >= 0.0f)
			return true;
		return false;
	}
	return false;
}

sshort eGamepad::GetButtons()
{
	return mState.Gamepad.wButtons;
}

short eGamepad::GetThumbLX()
{
	return mState.Gamepad.sThumbLX;
}

short eGamepad::GetThumbLY()
{
	return mState.Gamepad.sThumbLY;
}

short eGamepad::GetThumbRX()
{
	return mState.Gamepad.sThumbRX;
}

short eGamepad::GetThumbRY()
{
	return mState.Gamepad.sThumbRY;
}

eGamepadLayout eGamepad::LoadLayout(eString layout)
{
	strg filePath = "gamepad.rd";

	if (!EDR::FileExists(filePath.c_str()))
	{
		eConsole::PrintLog("WARNING::eGamepad::LoadLayout", "File 'gamepad.rd' could not be found!");
		return {};
	}

	eGamepadLayout rtVl;
	rtVl.A = (uint)EDR::GetIntG(filePath, "A", layout.GetStrg());
	rtVl.B = (uint)EDR::GetIntG(filePath, "B", layout.GetStrg());
	rtVl.X = (uint)EDR::GetIntG(filePath, "X", layout.GetStrg());
	rtVl.Y = (uint)EDR::GetIntG(filePath, "Y", layout.GetStrg());
	rtVl.DPad_up = (uint)EDR::GetIntG(filePath, "DPad_up", layout.GetStrg());
	rtVl.DPad_down = (uint)EDR::GetIntG(filePath, "DPad_down", layout.GetStrg());
	rtVl.DPad_left = (uint)EDR::GetIntG(filePath, "DPad_left", layout.GetStrg());
	rtVl.DPad_right = (uint)EDR::GetIntG(filePath, "DPad_right", layout.GetStrg());
	rtVl.LBShoulder = (uint)EDR::GetIntG(filePath, "LBShoulder", layout.GetStrg());
	rtVl.RBShoulder = (uint)EDR::GetIntG(filePath, "RBShoulder", layout.GetStrg());
	rtVl.LSThumbstick = (uint)EDR::GetIntG(filePath, "LSThumbstick", layout.GetStrg());
	rtVl.RSThumbstick = (uint)EDR::GetIntG(filePath, "RSThumbstick", layout.GetStrg());
	rtVl.Start = (uint)EDR::GetIntG(filePath, "Start", layout.GetStrg());
	rtVl.Back = (uint)EDR::GetIntG(filePath, "Back", layout.GetStrg());
	return rtVl;
}