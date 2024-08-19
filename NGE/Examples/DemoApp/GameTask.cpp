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
	mWnd->SetTitle("NGE Project");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	bQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);
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