/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef LAYER_HPP
#define LAYER_HPP
#include <Types.hpp>
#include <String.hpp>
#include "Viewport3D.hpp"
#include "Texture.hpp"
#include <bgfx/bgfx.h>

typedef bgfx::FrameBufferHandle  eFramebufferHandle;

class eWindow;

struct eLayerProperties
{
	bgfx::ViewId		mViewId;
	vec2i				mPosition;
	vec2				mResolution;
	vec2				mAspectRatio;
	bool				mTopMost;
	uint32				mClearColor;
	bool				mTransparent;
	bool				mUpdateSize;
	eString				mName;
	bool				mIs2D;
	bool				mDepthOnly;
	eWindow*			mWnd = nullptr;
};

struct eFramebuffer
{
	eFramebufferHandle	mFbh;
	eTextureHandle		mTexture;
	eTextureHandle		mDepthMap;
	vec2i				mTexSize;
};

class eLayer
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

	bool				mUseFramebuffer;

	void DestroyFB();
	void SetFBViewId(bgfx::ViewId viewId);
	void UpdateFB(vec2i texSize, eTextureFormat format = eTextureFormat::RGBA8);
};
#endif