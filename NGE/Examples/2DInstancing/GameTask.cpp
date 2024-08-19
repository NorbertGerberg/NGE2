/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "GameRenderer.hpp"
#include "GameTask.hpp"

//----------------------------------------------------2D Instancing example
eIDB mIdb;
//----------------------------------------------------End

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 320;
	uint32 height = 180;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("2D Instancing Example");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();
	mWnd->SetWindowAspectRatio(vec2i(16, 9));
	mWnd->SetWindowSizeLimits(vec2i(320, 180), vec2i(1920, 1080));

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	bQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);

	mTexture = new eTexture();
	mTexture->Load("tex_block.png", false, 20);

	//----------------------------------------------------2D Instancing example
	eTransformation2D trans;
	trans.mPosition = vec2(0.0f);
	trans.mRotation = 0.0f;
	trans.mRotationPivot = vec2(0.0f);
	trans.mScale = vec2(16.0f);

	std::vector<eTransformation2D> mTrans;

	for (int i = 0; i < 100; i++)
	{
		mTrans.push_back(trans);
		trans.mPosition.x += 16.0f;
		trans.mRotation += 2.5f;
	}
	trans.mPosition = vec2(0.0f);
	trans.mRotation = 0.0f;

	for (int i = 0; i < 100; i++)
	{
		mTrans.push_back(trans);
		trans.mPosition.y += 16.0f;
		trans.mRotation -= 2.5f;
	}
	trans.mPosition = vec2(16.0f);
	trans.mRotation = 0.0f;

	for (int i = 1; i < 100; i++)
	{
		mTrans.push_back(trans);
		trans.mPosition.x += 16.0f;
		trans.mPosition.y += 16.0f;
	}

	mRenderer->GetGfx()->InitIDB(mRenderer->GetLayer("2D"), mTrans, mIdb);
	mTrans.clear();
	//----------------------------------------------------End

	return true;
}

void GameTask::Update()
{
	mTime->Update();
	bQuit = mWnd->WindowCloseRequested();

	UpdateFixedTime();
	while (mAccumulatedTime >= mFixedTime)
	{
		//	Fixed timestep
		//	Update code here!
		PostUpdateFixedTime();
	}
}

void GameTask::Render()
{
	if (!mWnd->IsWindowIconified())
	{
		mRenderer->PreUpdate(true, 16);

		//----------------------------------------------------2D Instancing example
		
		eGraphics* gfx	= mRenderer->GetGfx();
		eShader* shader = mRenderer->GetShader("Sprite2DInstancing");
		eLayer* layer	= mRenderer->GetLayer("2D");

		eViewport2D vp2d;
		vp2d.SetTranslation(vec2(0.0f));
		vp2d.SetSize(mWnd->GetWindowSize());
		real3 py = vp2d.GetSize().y - 128.0f;

		gfx->Draw2DInstances(shader, layer, vp2d, mIdb, mTexture);

		//----------------------------------------------------End

		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	if (mTexture != nullptr)
		delete mTexture;

	if (mRenderer != nullptr)
	{
		mRenderer->SetWindow(nullptr);
		delete mRenderer;
		mRenderer = nullptr;
	}

	mTime->SetSpeed(nullptr);

	SuperUnload();
}