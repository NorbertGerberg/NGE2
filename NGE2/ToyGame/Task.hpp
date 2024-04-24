/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef TASK_HPP
#define TASK_HPP
#include <Types.hpp>
#include "Time.hpp"
#include "Window.hpp"
#include "Gamepad.hpp"
#include "Audio.hpp"

class eTask
{
public:
	virtual bool Initialize()	= 0;
	virtual void Update()		= 0;
	virtual void Render()		= 0;
	virtual void Unload()		= 0;

	bool SuperInit();
	void SuperUnload();

	eTime*		GetTime();
	eWindow*	GetWindow();
	eGamepad*	GetGamepad();
	eAudio*		GetAudio();

protected:
	eTime*		mTime;
	eWindow*	mWnd;
	eGamepad*	mGamepad;
	eAudio*		mAudio;
};
#endif