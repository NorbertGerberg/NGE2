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
	mGfx->CompileShader("Unlit");
	PushShader("Unlit");
	eShader* shd = GetShader("Unlit");
	shd->InitUniform("tex_diffuse", eUniformType::Sampler);
	shd = nullptr;

	//	Init Layers
	PushLayer("3D", false);
	m3DPass = GetLayer("3D");
	eLayerProperties& prp	= m3DPass->GetProperties();
	prp.mViewId				= 0;
	prp.mName				= "3DPass";
	prp.mTopMost			= true;
	prp.mIs2D				= false;
	prp.mClearColor			= 0;
	prp.mPosition			= vec2i(0);
	prp.mResolution			= vec2(static_cast<real3>(width), static_cast<real3>(height));
	prp.mAspectRatio		= vec2(static_cast<real3>(width), static_cast<real3>(height));
	prp.mUpdateSize			= true;
	prp.mTransparent		= false;
	prp.mDepthOnly			= false;
	m3DPass->Initialize();
}

GameRenderer::~GameRenderer()
{
	m3DPass	= nullptr;
}

void GameRenderer::PreUpdate(bool vsync, uint8 msaa)
{
	vec2i size = UpdateSize(mWnd, vsync, msaa);
	m3DPass->Update(size.x, size.y);
}

void GameRenderer::Update()
{
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