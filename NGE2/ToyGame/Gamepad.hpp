/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Windows.h>
#include <Xinput.h>

struct eGamepadLayout
{
	uint A = 4096;
	uint B = 8192;
	uint X = 16384;
	uint Y = 32768;
	uint DPad_up = 1;
	uint DPad_down = 2;
	uint DPad_left = 4;
	uint DPad_right = 8;
	uint LBShoulder = 256;
	uint RBShoulder = 512;
	uint LSThumbstick = 64;
	uint RSThumbstick = 128;
	uint Start = 16;
	uint Back = 32;
};

struct eDeadzoneStick
{
	real6 leftX;
	real6 leftY;
	real6 rightX;
	real6 rightY;
};

class eGamepad
{
public:
	bool IsConnected(ulong port);
	eDeadzoneStick DeadzoneCorrection();
	void Vibration(ulong port, int leftmotorspeed, int rightmotorspeed);
	bool GetButtonPressed(uint button);
	bool GetButtonReleased(uint button);
	sshort GetButtons();
	short GetThumbLX();
	short GetThumbLY();
	short GetThumbRX();
	short GetThumbRY();
	eGamepadLayout LoadLayout(eString layout);

private:
	XINPUT_STATE mState;
	XINPUT_VIBRATION mVibration;
};
#endif