/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef GAMETASK_HPP
#define GAMETASK_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Task.hpp>
#include <Audio.hpp>

class GameRenderer;

class GameTask : public eTask
{
public:
	bool Initialize();
	void Update();
	void Render();
	void Unload();

	

private:
	real3			mGameSpeed;
	GameRenderer*	mRenderer;
	eAudioSrc*		mAudioSrc;
};
#endif