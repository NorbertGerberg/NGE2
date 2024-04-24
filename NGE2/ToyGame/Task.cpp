/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Task.hpp"

bool eTask::SuperInit()
{
	mTime		= new eTime();
	mGamepad	= new eGamepad();
	mWnd		= new eWindow();
	mAudio		= new eAudio();
	return mAudio->Initialize();
}

void eTask::SuperUnload()
{
	if (mTime != nullptr)
		delete mTime;

	if (mGamepad != nullptr)
		delete mGamepad;

	if (mWnd != nullptr)
		delete mWnd;

	if (mAudio != nullptr)
		delete mAudio;
}

eTime* eTask::GetTime()
{
	return mTime;
}

eWindow* eTask::GetWindow()
{
	return mWnd;
}

eGamepad* eTask::GetGamepad()
{
	return mGamepad;
}

eAudio* eTask::GetAudio()
{
	return mAudio;
}