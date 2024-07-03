/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "GameRenderer.hpp"
#include <Window.hpp>

GameRenderer::GameRenderer(eWindow* window, uint32 width, uint32 height, bool vsync, uint8 msaa, eRenderAPI api)
	: eRenderer(window->GetWnd(), width, height, vsync, msaa, api)
{
	mWnd = window;

	//	Init Shaders
	mGfx->CompileShader("Sprite2DInstancing");
	PushShader("Sprite2DInstancing");
	eShader* shd = GetShader("Sprite2DInstancing");
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

	mGfx->InitDI(mWnd->GetGLFWwindow(), NULL);
}

GameRenderer::~GameRenderer()
{
	mGfx->ReleaseDI();
	m2DPass = nullptr;
}

void GameRenderer::PreUpdate(bool vsync, uint8 msaa)
{
	vec2i size = UpdateSize(mWnd, vsync, msaa);
	m2DPass->Update(size);
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