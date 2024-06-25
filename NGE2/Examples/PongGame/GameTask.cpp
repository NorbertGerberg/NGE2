/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/GameRenderer.hpp"
#include "GameTask.hpp"
#include <Texture.hpp>
#include <Keyboard.hpp>
#include <Collision.hpp>

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 640;
	uint32 height = 420;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("Pong Game");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();
	mWnd->SetWindowResizable(false);

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	mQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);

	mBlockTex = new eTexture();
	mBlockTex->Load("tex_block.png", false, 20);

	mPlayerTrans.mPosition		= vec2(16.0f);
	mPlayerTrans.mRotation		= 0.0f;
	mPlayerTrans.mRotationPivot = vec2(0.0f);
	mPlayerTrans.mScale			= vec2(16.0f, 64.0f);
	mPlayerCol.mPos				= mPlayerTrans.mPosition;
	mPlayerCol.mSize			= mPlayerTrans.mScale;

	mCOMTrans.mPosition			= vec2(608.0f, 16.0f);
	mCOMTrans.mRotation			= 0.0f;
	mCOMTrans.mRotationPivot	= vec2(0.0f);
	mCOMTrans.mScale			= vec2(16.0f, 64.0f);
	mCOMmov						= 1.0f;

	mBallTrans.mPosition		= vec2(32.0f, 16.0f);
	mBallTrans.mRotation		= 0.0f;
	mBallTrans.mRotationPivot	= vec2(0.0f);
	mBallTrans.mScale			= vec2(16.0f);
	mBALLmov					= 1.0f;
	mBallCol.mPos				= mBallTrans.mPosition;
	mBallCol.mSize				= mBallTrans.mScale;

	mFontTex = new eTexture();
	mFontTex->Load("tex_font_default.bmp", false, 20);

	mScore = 0;
	mBOnce = false;
	return true;
}

void GameTask::Update()
{
	mTime->Update();
	mQuit = mWnd->WindowCloseRequested();

	UpdateFixedTime();
	while (mAccumulatedTime >= mFixedTime)
	{
		//	COM
		mCOMTrans.mPosition.y += 5.0f * mCOMmov;
		if (mCOMTrans.mPosition.y >= 356.0f || mCOMTrans.mPosition.y <= 16.0f)
			mCOMmov *= -1.0f;

		//	PLAYER
		vec2 playerPos = mPlayerCol.mPos;

		if (mWnd->GetKeyboard()->GetKeyDown(NGE_INPUT_KEY_UP))
			playerPos.y -= 10.0f;
		else if (mWnd->GetKeyboard()->GetKeyDown(NGE_INPUT_KEY_DOWN))
			playerPos.y += 10.0f;

		if (playerPos.y >= 356.0f)
			playerPos.y = 356.0f;
		else if (playerPos.y <= 16.0f)
			playerPos.y = 16.0f;

		mPlayerTrans.mPosition	= playerPos;
		mPlayerCol.mPos			= playerPos;

		//	BALL
		vec2 ballPos = mBallCol.mPos;

		ballPos.x += 5.0f * mBALLmov;
		if (ballPos.x <= 0.0f)
			mQuit = true;
		else if (ballPos.x >= 592.0f)
			mBALLmov *= -1.0f;

		mBallTrans.mPosition	= ballPos;
		mBallCol.mPos			= vec2(ballPos.x, mCOMTrans.mPosition.y);

		if (eCollision::AABB2DInAABB2D(mPlayerCol, mBallCol))
		{
			if (!mBOnce)
			{
				mScore++;
				mBALLmov *= -1.0f;
			}
			mBOnce = true;
		}
		else
			mBOnce = false;

		PostUpdateFixedTime();
	}
}

void GameTask::Render()
{
	if (!mWnd->IsWindowIconified())
	{
		mRenderer->PreUpdate(true, 16);

		eGraphics* gfx	= mRenderer->GetGfx();
		eShader* shader = mRenderer->GetShader("Sprite2D");
		eLayer* layer	= mRenderer->GetLayer("2D");

		eViewport2D vp2d;
		vp2d.SetTranslation(vec2(0.0f));
		vp2d.SetSize(mWnd->GetWindowSize());

		gfx->Draw2D(shader, layer, vp2d, mPlayerTrans, mBlockTex);
		gfx->Draw2D(shader, layer, vp2d, mCOMTrans, mBlockTex);
		gfx->Draw2D(shader, layer, vp2d, mBallTrans, mBlockTex);


		eShader* shaderTxt = mRenderer->GetShader("Text2D");

		eTextureAtlas atlas;
		atlas.mSize = vec2(319.0f, 64.0f);
		atlas.mSubSize = vec2(11.0f, 16.0f);
		atlas.mTexture = mFontTex;

		eTextAtlas textAtlas;
		textAtlas.mAtlas = atlas;
		textAtlas.mSpace = vec2(0.7f, 1.0f);

		eTransformation2D trans;
		trans.mPosition = vec2(96.0f, 64.0f);
		trans.mRotation = 0.0f;
		trans.mRotationPivot = vec2(0.0f);
		trans.mScale = vec2(11.0f, 16.0f) * 2.0f;

		gfx->DrawText(shaderTxt, layer, vp2d, trans, textAtlas, "Points: " + std::to_string(mScore));

		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	if (mBlockTex != nullptr)
	{
		delete mBlockTex;
		mBlockTex = nullptr;
	}

	if (mFontTex != nullptr)
	{
		delete mFontTex;
		mFontTex = nullptr;
	}

	if (mRenderer != nullptr)
	{
		mRenderer->SetWindow(nullptr);
		delete mRenderer;
		mRenderer = nullptr;
	}

	mTime->SetSpeed(nullptr);

	SuperUnload();
}