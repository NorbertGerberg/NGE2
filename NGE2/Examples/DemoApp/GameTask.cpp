/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/GameRenderer.hpp"
#include "GameTask.hpp"

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 1280;
	uint32 height = 720;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("ToyGame Project");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	mQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);

	mAccumulatedTime = 0.0;

	return true;
}

void GameTask::Update()
{
	mTime->Update();
	mQuit = mWnd->WindowCloseRequested();

	const real6 fixedTime = 0.01666667;			// ~60 FPS (1 / 60)
	mAccumulatedTime += mTime->DeltaTime();

	while (mAccumulatedTime >= fixedTime)
	{
		//	Fixed timestep
		//	Update code here!
		mAccumulatedTime -= fixedTime;
	}
}

void GameTask::Render()
{
	if (!mWnd->IsWindowIconified())
	{
		mRenderer->PreUpdate(true, 16);
		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	if (mRenderer != nullptr)
	{
		mRenderer->SetWindow(nullptr);
		delete mRenderer;
		mRenderer = nullptr;
	}

	mTime->SetSpeed(nullptr);

	SuperUnload();
}