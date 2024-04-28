/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Task.hpp"

bool eTask::SuperInit()
{
	mTime				= new eTime();
	mGamepad			= new eGamepad();
	mWnd				= new eWindow();
	mAudio				= new eAudio();
	mAccumulatedTime	= 0.0;
	mFixedTime			= 0.01666667;		// ~60 FPS (1 / 60)
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

	mAccumulatedTime	= 0.0;
	mFixedTime			= 0.0;
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

void eTask::SetFixedTime(const real6 framerate)
{
	mFixedTime = (1.0 / framerate);
}

void eTask::UpdateFixedTime()
{
	mAccumulatedTime += mTime->DeltaTime();
}

void eTask::PostUpdateFixedTime()
{
	mAccumulatedTime -= mFixedTime;
}