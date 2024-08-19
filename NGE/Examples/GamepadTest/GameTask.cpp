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
	mWnd->SetTitle("Gamepad Test");
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
		//----------------------------------------------------Gamepad Test
		if (mGamepad->IsConnected(0))
		{
			eDeadzoneStick dz = mGamepad->DeadzoneCorrection();

			if (dz.leftX >= 0.9f)
				mGamepad->Vibration(0, 0, 20000);
			else if (dz.leftX <= -0.9f)
				mGamepad->Vibration(0, 20000, 0);
			else
				mGamepad->Vibration(0, 0, 0);

			eGamepadLayout layout;
			if (mGamepad->GetButtonPressed(layout.A))
				bQuit = true;
		}
		//----------------------------------------------------End
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