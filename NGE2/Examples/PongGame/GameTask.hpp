/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef GAMETASK_HPP
#define GAMETASK_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Task.hpp>
#include <Transformations.hpp>
#include <Collider.hpp>

class GameRenderer;
class eTexture;

class GameTask : public eTask
{
public:
	bool Initialize();
	void Update();
	void Render();
	void Unload();

	bool				mQuit;

private:
	real3				mGameSpeed;
	GameRenderer*		mRenderer;

	eTexture*			mBlockTex;
	eTransformation2D	mPlayerTrans;
	eCollider::eAABB2D	mPlayerCol;

	eTransformation2D	mCOMTrans;
	real3				mCOMmov;

	eTransformation2D	mBallTrans;
	real3				mBALLmov;
	eCollider::eAABB2D	mBallCol;

	eTexture*			mFontTex;
	uint16				mScore;
	bool				mBOnce;
};
#endif