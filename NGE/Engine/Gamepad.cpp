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
	real6 normalizedLX = 0.0;
	real6 normalizedLY = 0.0;

	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		magnitude = max(0, magnitude - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		real6 normalizedMagnitude = magnitude / (32767.0 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		normalizedLX = LX / 32767.0;
		normalizedLY = LY / 32767.0;
	}

	real6 RX = mState.Gamepad.sThumbRX;
	real6 RY = mState.Gamepad.sThumbRY;

	real6 Rmagnitude = sqrt(RX * RX + RY * RY);
	real6 normalizedRX = 0.0;
	real6 normalizedRY = 0.0;

	if (Rmagnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		Rmagnitude = max(0, Rmagnitude - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		real6 RnormalizedMagnitude = Rmagnitude / (32767.0 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		normalizedRX = RX / 32767.0;
		normalizedRY = RY / 32767.0;
	}

	return { normalizedLX, normalizedLY, normalizedRX, normalizedRY };
}

void eGamepad::Vibration(ulong port, int leftmotorspeed, int rightmotorspeed)
{
	mVibration.wLeftMotorSpeed = leftmotorspeed;
	mVibration.wRightMotorSpeed = rightmotorspeed;
	XInputSetState(port, &mVibration);
}

bool eGamepad::GetButtonPressed(uint button)
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

bool eGamepad::GetButtonDown(uint button)
{
	return (mState.Gamepad.wButtons & button);
}

bool eGamepad::GetButtonReleased(uint button)
{
	return !(mState.Gamepad.wButtons & button);
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