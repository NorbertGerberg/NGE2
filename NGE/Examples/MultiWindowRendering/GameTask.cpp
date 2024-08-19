/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "GameRenderer.hpp"
#include "GameTask.hpp"

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 1280;
	uint32 height = 720;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("Multi Window Rendering");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();

	//----------------------------------------------------Multi Window Rendering
	mWnd2 = new eWindow();
	mWnd2->SetSize(vec2i(320, 180));
	mWnd2->SetTitle("Second Window");
	mWnd2->Initialize();
	mWnd2->SetWindowVisibility(true);
	mWnd2->SetWindowPosition(vec2i(400));

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11, mWnd2);
	//----------------------------------------------------End

	bQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);

	mBlockTex = new eTexture();
	mBlockTex->Load("tex_block.png", false, 20);
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

		//----------------------------------------------------Multi Window Rendering
		eGraphics* gfx = mRenderer->GetGfx();
		eShader* shader = mRenderer->GetShader("Sprite2D");
		eLayer* layer = mRenderer->GetLayer("2D");
		eLayer* layer2 = mRenderer->GetLayer("2D2");

		eViewport2D vp2d;
		vp2d.SetTranslation(vec2(0.0f));
		vp2d.SetSize(mWnd->GetWindowSize());

		eTransformation2D trans;
		trans.mPosition = vec2(16.0f);
		trans.mRotation = 0.0f;
		trans.mRotationPivot = vec2(0.5f);
		trans.mScale = vec2(64.0f);

		gfx->Draw2D(shader, layer, vp2d, trans, mBlockTex);

		vp2d.SetSize(mWnd2->GetWindowSize());
		trans.mPosition = vec2(128.0f, 64.0f);
		trans.mRotation = 12.0f;
		gfx->Draw2D(shader, layer2, vp2d, trans, mBlockTex);
		//----------------------------------------------------End

		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	if (mBlockTex != nullptr)
		delete mBlockTex;

	if (mRenderer != nullptr)
	{
		mRenderer->SetWindow(nullptr);
		delete mRenderer;
		mRenderer = nullptr;
	}

	mTime->SetSpeed(nullptr);

	//----------------------------------------------------Multi Window Rendering
	if (mWnd2 != nullptr)
		delete mWnd2;
	//----------------------------------------------------End

	SuperUnload();
}