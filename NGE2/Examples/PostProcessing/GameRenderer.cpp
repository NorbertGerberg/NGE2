/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "GameRenderer.hpp"
#include <Window.hpp>
#include <Tools.hpp>

GameRenderer::GameRenderer(eWindow* window, uint32 width, uint32 height, bool vsync, uint8 msaa, eRenderAPI api)
	: eRenderer(window->GetWnd(), width, height, vsync, msaa, api)
{
	mWnd = window;

	//	Init Shaders
	mGfx->CompileShader("PostProcessing");
	PushShader("PostProcessing");
	eShader* shd = GetShader("PostProcessing");
	shd->InitUniform("properties", eUniformType::Vec4);
	shd->InitUniform("s_texColor", eUniformType::Sampler);
	shd = nullptr;

	mGfx->CompileShader("Unlit");
	PushShader("Unlit");
	shd = GetShader("Unlit");
	shd->InitUniform("tex_diffuse", eUniformType::Sampler);
	shd = nullptr;

	//	Init Layers
		//----------------------------------------------------PostProcessing Test
	PushLayer("Base", false);
	mBasePass = GetLayer("Base");
	eLayerProperties& prp	= mBasePass->GetProperties();
	prp.mViewId				= 0;
	prp.mName				= "BasePass";
	prp.mTopMost			= true;
	prp.mIs2D				= true;
	prp.mClearColor			= 0;
	prp.mPosition			= vec2i(0);
	prp.mResolution			= vec2(static_cast<real3>(width), static_cast<real3>(height));
	prp.mAspectRatio		= vec2(static_cast<real3>(width), static_cast<real3>(height));
	prp.mUpdateSize			= true;
	prp.mTransparent		= false;
	prp.mDepthOnly			= false;
	mBasePass->Initialize();

	PushLayer("3D");
	m3DPass = GetLayer("3D");
	eLayerProperties& prp1	= m3DPass->GetProperties();
	prp1.mViewId			= 1;
	prp1.mName				= "3DPass";
	prp1.mTopMost			= true;
	prp1.mIs2D				= false;
	prp1.mClearColor		= 0;
	prp1.mPosition			= vec2i(0);
	prp1.mResolution		= vec2(static_cast<real3>(width), static_cast<real3>(height));
	prp1.mAspectRatio		= vec2(static_cast<real3>(width), static_cast<real3>(height));
	prp1.mUpdateSize		= true;
	prp1.mTransparent		= false;
	prp1.mDepthOnly			= false;
	m3DPass->Initialize();
	//----------------------------------------------------End
}

GameRenderer::~GameRenderer()
{
	//----------------------------------------------------PostProcessing Test
	mBasePass	= nullptr;
	//----------------------------------------------------End
	m3DPass		= nullptr;
}

void GameRenderer::PreUpdate(bool vsync, uint8 msaa)
{
	vec2i size = UpdateSize(mWnd, vsync, msaa);

	m3DPass->Update(size);
	//----------------------------------------------------PostProcessing Test
	mBasePass->Update(size);
	//----------------------------------------------------End
}

void GameRenderer::Update()
{
	//----------------------------------------------------PostProcessing Test
	vec2i tSize = m3DPass->GetFramebuffer().mTexSize;

	eTransformation2D tr;
	tr.mPosition		= vec2(0.0f);
	tr.mRotation		= 0.0f;
	tr.mRotationPivot	= vec2(0.0f);
	tr.mScale			= vec2(tSize);

	eViewport2D vp;
	vp.SetTranslation(vec2(0.0f));
	vp.SetSize(vec2(tSize));

	eTexture* tmpTex = new eTexture();
	tmpTex->SetHandle(eTools::Framebuffer2Texture(m3DPass->GetFramebuffer().mTexture, tSize,
		eTextureFormat::RGBA8, BGFX_SAMPLER_COMPARE_LEQUAL));

	const real3 fxaa = 1.0f;
	GetShader("PostProcessing")->SetUniform("properties", eMath::value_ptr(vec4(static_cast<real3>(fxaa), 0, 0, 0)));
	mGfx->Draw2D(GetShader("PostProcessing"), mBasePass, vp, tr, tmpTex);
	delete tmpTex;
	//----------------------------------------------------End

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