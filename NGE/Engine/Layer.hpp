/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef LAYER_HPP
#define LAYER_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
#include "Viewport3D.hpp"
#include "Texture.hpp"
#include <bgfx/bgfx.h>

typedef bgfx::FrameBufferHandle  eFramebufferHandle;

class eWindow;

//! Layer properties
//! \ingroup Engine
struct eLayerProperties
{
	bgfx::ViewId		mViewId;
	vec2i				mPosition;
	vec2				mResolution;
	vec2				mAspectRatio;
	bool				bTopMost;
	uint32				mClearColor;
	bool				bTransparent;
	bool				bUpdateSize;
	eString				mName;
	bool				bIs2D;
	bool				bDepthOnly;
	eWindow*			mWnd = nullptr;
};

//! Framebuffer properties
//! \ingroup Engine
struct eFramebuffer
{
	eFramebufferHandle	mFbh;
	eTextureHandle		mTexture;
	eTextureHandle		mDepthMap;
	vec2i				mTexSize;
};

//! Layer class
//! \ingroup Engine
class eLayer : public eBase
{
public:
	eLayer(bool useFramebuffer = true);
	~eLayer();
	bool Initialize();
	void Unload();
	void Update(vec2 size);

	void SetViewport(eViewport3D* viewport);
	eViewport3D* GetViewport();

	eLayerProperties& GetProperties();
	eFramebuffer& GetFramebuffer();

	static bool mRefreshSize;

private:
	eViewport3D*		mViewport;
	eLayerProperties	mProperties;
	eFramebuffer		mFramebuffer;

	bool				bUseFramebuffer;

	void DestroyFB();
	void SetFBViewId(bgfx::ViewId viewId);
	void UpdateFB(vec2i texSize, eTextureFormat format = eTextureFormat::RGBA8);
};
#endif