/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/GameRenderer.hpp"
#include "GameTask.hpp"

#include <ComponentFactory.hpp>
#include "MyComponent.hpp"

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 1280;
	uint32 height = 720;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("Alarm Test");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	mQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);


	//	Register component class
	eComponentFactory::RegisterType<MyComponent>("MyComponent");

	//	Create actor prefab
	eActor* newActor = new eActor();
	newActor->SetName("My Actor");
	newActor->SetClass("New Actor");
	newActor->SetDetailLvl(eQuality::eVeryLow);

	newActor->PushComponent("MyComponent", "First Component");
	if (!newActor->Initialize())
		return false;
	newActor->GetComponent("First Component")->SetProperty("Message", eString("Alarm test!"));

	newActor->SavePrefab("NewActor.actor");
	newActor->Unload();
	delete newActor;
	newActor = nullptr;

	//	Init scene
	mScene = new eScene();
	mScene->SetGlobalQuality(eQuality::eHigh);

	//	Create a scene and save it
	mScene->SetName("My Scene");
	mScene->SpawnActor("NewActor.actor", "Spawned Actor");
	if (!mScene->Initialize())
		return false;
	mScene->Save("MyScene.scene");

	return true;
}

void GameTask::Update()
{
	mTime->Update();
	mQuit = mWnd->WindowCloseRequested();

	UpdateFixedTime();
	while (mAccumulatedTime >= mFixedTime)
	{
		mScene->Update();
		PostUpdateFixedTime();
	}
}

void GameTask::Render()
{
	if (!mWnd->IsWindowIconified())
	{
		mRenderer->PreUpdate(true, 16);
			mScene->Render();
		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	if (mScene != nullptr)
	{
		delete mScene;
		mScene = nullptr;
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