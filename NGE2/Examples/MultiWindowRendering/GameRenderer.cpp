/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "GameRenderer.hpp"
#include <Window.hpp>

GameRenderer::GameRenderer(eWindow* window, uint32 width, uint32 height, bool vsync, uint8 msaa, eRenderAPI api, eWindow* window2)
	: eRenderer(window->GetWnd(), width, height, vsync, msaa, api)
{
	mWnd = window;
	//----------------------------------------------------Multi Window Rendering
	mWnd2 = window2;
	//----------------------------------------------------End

	//	Init Shaders
	PushShader("Sprite2D");
	eShader* shd = GetShader("Sprite2D");
	shd->InitUniform("atlasInfo", eUniformType::Vec4, 2);
	shd->InitUniform("s_texColor", eUniformType::Sampler);
	shd = nullptr;

	//mGfx->CompileShader("Text2D");
	PushShader("Text2D");
	shd = GetShader("Text2D");
	shd->InitUniform("atlasInfo", eUniformType::Vec4, 2);
	shd->InitUniform("s_texColor", eUniformType::Sampler);
	shd = nullptr;

	//	Init Layers
	PushLayer("2D", false);
	m2DPass = GetLayer("2D");
	eLayerProperties& prp = m2DPass->GetProperties();
	prp.mViewId = 0;
	prp.mName = "2DPass";
	prp.mTopMost = true;
	prp.mIs2D = true;
	prp.mClearColor = 0;
	prp.mPosition = vec2i(0);
	prp.mResolution = vec2(static_cast<real3>(width), static_cast<real3>(height));
	prp.mAspectRatio = vec2(static_cast<real3>(width), static_cast<real3>(height));
	prp.mUpdateSize = true;
	prp.mTransparent = false;
	prp.mDepthOnly = false;
	m2DPass->Initialize();

	//----------------------------------------------------Multi Window Rendering
	PushLayer("2D2");
	m2DPass2 = GetLayer("2D2");
	eLayerProperties& prp2 = m2DPass2->GetProperties();
	prp2.mViewId = 1;
	prp2.mName = "2DPass2";
	prp2.mTopMost = true;
	prp2.mIs2D = true;
	prp2.mClearColor = 0;
	prp2.mPosition = vec2i(0);
	prp2.mResolution = window2->GetSize();
	prp2.mAspectRatio = window2->GetSize();
	prp2.mUpdateSize = true;
	prp2.mTransparent = false;
	prp2.mDepthOnly = false;
	prp2.mWnd		= window2;
	m2DPass2->Initialize();

	mGfx->InitDI(mWnd->GetGLFWwindow(), NULL, 2);
	//----------------------------------------------------End
}

GameRenderer::~GameRenderer()
{
	mGfx->ReleaseDI();
	m2DPass = nullptr;
	//----------------------------------------------------Multi Window Rendering
	m2DPass2 = nullptr;
	//----------------------------------------------------End
}

void GameRenderer::PreUpdate(bool vsync, uint8 msaa)
{
	vec2i size = UpdateSize(mWnd, vsync, msaa);
	m2DPass->Update(size);

	//----------------------------------------------------Multi Window Rendering
	mWnd2->SizeChanged();
	m2DPass2->Update(mWnd2->GetWindowSize());
	//----------------------------------------------------End

	mGfx->BeginDI();
}

void GameRenderer::Update()
{
	mGfx->EndDI();
	mGfx->Frame();
}

void GameRenderer::SetWindow(eWindow* window)
{
	mWnd = window;
}

eWindow* GameRenderer::GetWindow()
{
	return mWnd;
}