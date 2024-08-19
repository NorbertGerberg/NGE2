/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/GameRenderer.hpp"
#include "GameTask.hpp"

#include "MyDoer.hpp"

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 1280;
	uint32 height = 720;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("Scene Example");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	bQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);


	//----------------------------------------------------Scene example
	eWorld::INIT();
	mWorld = new eWorld();

	eWorld::Factory().Register<MyDoer>();

	if (EDR::FileExists("TEST.world"))
		mWorld->Load("TEST.world");
	else
		mWorld->InsertDoer<MyDoer>("Mine One");
	mWorld->Initialize();
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
		//----------------------------------------------------Scene example
		mWorld->Update();
		//----------------------------------------------------End
		PostUpdateFixedTime();
	}
}

void GameTask::Render()
{
	if (!mWnd->IsWindowIconified())
	{
		mRenderer->PreUpdate(true, 16);

		//----------------------------------------------------Scene example
		mWorld->Render();
		//----------------------------------------------------End

		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	//----------------------------------------------------Scene example
	if (mWorld != nullptr)
	{
		mWorld->Save("TEST.world");
		delete mWorld;
		mWorld = nullptr;
	}
	eWorld::PopPropertyTypes();
	//----------------------------------------------------End

	if (mRenderer != nullptr)
	{
		mRenderer->SetWindow(nullptr);
		delete mRenderer;
		mRenderer = nullptr;
	}

	mTime->SetSpeed(nullptr);

	SuperUnload();
}