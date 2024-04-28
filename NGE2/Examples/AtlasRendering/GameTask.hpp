/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef GAMETASK_HPP
#define GAMETASK_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Task.hpp>

class GameRenderer;
class eTexture;

class GameTask : public eTask
{
public:
	bool Initialize();
	void Update();
	void Render();
	void Unload();

	bool			mQuit;

private:
	real3			mGameSpeed;
	GameRenderer*	mRenderer;

	//----------------------------------------------------Atlas example
	eTexture*		mTexture;
	//----------------------------------------------------End
};
#endif