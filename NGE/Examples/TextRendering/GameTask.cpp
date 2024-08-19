/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/GameRenderer.hpp"
#include "GameTask.hpp"

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 320;
	uint32 height = 180;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("Text Rendering Example");
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

	//----------------------------------------------------Text example
	mTexture = new eTexture();
	mTexture->Load("tex_font_default_big.png", false, 20);
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

		//----------------------------------------------------Text example
		
		eGraphics* gfx	= mRenderer->GetGfx();
		eShader* shader = mRenderer->GetShader("Text2D");
		eLayer* layer	= mRenderer->GetLayer("2D");

		eTextureAtlas atlas;
		atlas.mSize = vec2(1015.0f, 116.0f);
		atlas.mSubSize = vec2(35.0f, 29.0f);
		atlas.mTexture = mTexture;

		eTextAtlas textAtlas;
		textAtlas.mAtlas = atlas;
		textAtlas.mSpace = vec2(0.7f, 1.7f);

		eViewport2D vp2d;
		vp2d.SetTranslation(vec2(0.0f));
		vp2d.SetSize(mWnd->GetWindowSize());
		real3 py = vp2d.GetSize().y - 128.0f;

		eTransformation2D trans;
		trans.mPosition = vec2(5.0f);
		trans.mRotation = 0.0f;
		trans.mRotationPivot = vec2(0.0f);
		trans.mScale = vec2(35.0f, 29.0f) * 0.64f;

		gfx->DrawString(shader, layer, vp2d, trans, textAtlas, "Hello, World!\nNGE2!");

		//----------------------------------------------------End

		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	//----------------------------------------------------Text example
	if (mTexture != nullptr)
		delete mTexture;
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