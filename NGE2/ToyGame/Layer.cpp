/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Layer.hpp"
#include <bx/bx.h>
#include <bx/math.h>

bool eLayer::mRefreshSize = true;

eLayer::eLayer(bool useFramebuffer)
{
	mFramebuffer.mFbh = BGFX_INVALID_HANDLE;
	mFramebuffer.mTexture = BGFX_INVALID_HANDLE;
	mFramebuffer.mDepthMap = BGFX_INVALID_HANDLE;
	mUseFramebuffer = useFramebuffer;
}

eLayer::~eLayer()
{
	Unload();
}

bool eLayer::Initialize()
{
	bgfx::setViewMode(mProperties.mViewId, bgfx::ViewMode::DepthDescending);
	UpdateFB(vec2i(static_cast<int>(mProperties.mResolution.x), static_cast<int>(mProperties.mResolution.y)));
	SetFBViewId(mProperties.mViewId);
	bgfx::setViewRect(mProperties.mViewId, (uint16)mProperties.mPosition.x, (uint16)mProperties.mPosition.y, static_cast<uint16>(mProperties.mResolution.x), static_cast<uint16>(mProperties.mResolution.y));
	bgfx::setViewScissor(mProperties.mViewId, (uint16)mProperties.mPosition.x, (uint16)mProperties.mPosition.y, static_cast<uint16>(mProperties.mResolution.x), static_cast<uint16>(mProperties.mResolution.y));
	uint16 flags = BGFX_CLEAR_NONE;
	if (mProperties.mTopMost)
		flags |= BGFX_CLEAR_DEPTH;
	if (!mProperties.mTransparent)
		flags |= BGFX_CLEAR_COLOR;

	bgfx::setViewClear(mProperties.mViewId, flags, mProperties.mClearColor, 1.0f, 0);
	return true;
}

void eLayer::Unload()
{
	DestroyFB();
}

void eLayer::Update(int width, int height)
{
	if (mRefreshSize)
	{
		if (mProperties.mUpdateSize)
		{
			mProperties.mResolution = vec2(width, height);
			mProperties.mAspectRatio = mProperties.mResolution;
		}

		if (bgfx::isValid(mFramebuffer.mFbh))
		{
			DestroyFB();
			UpdateFB(vec2i(static_cast<int>(mProperties.mResolution.x), static_cast<int>(mProperties.mResolution.y)));
			SetFBViewId(mProperties.mViewId);
		}

		bgfx::setViewRect(mProperties.mViewId, (uint16)mProperties.mPosition.x, (uint16)mProperties.mPosition.y, static_cast<uint16>(mProperties.mResolution.x), static_cast<uint16>(mProperties.mResolution.y)); //bgfx::BackbufferRatio::Equal
		bgfx::setViewScissor(mProperties.mViewId, (uint16)mProperties.mPosition.x, (uint16)mProperties.mPosition.y, static_cast<uint16>(mProperties.mResolution.x), static_cast<uint16>(mProperties.mResolution.y));
	}

	if (!mProperties.mIs2D)
	{
		if (mViewport != nullptr)
			mViewport->Update(mProperties.mViewId, mProperties.mAspectRatio.x / mProperties.mAspectRatio.y, 0.1f, 9999999.0f);
	}

	uint16 flags = BGFX_CLEAR_NONE;
	if (mProperties.mTopMost)
		flags |= BGFX_CLEAR_DEPTH;
	if (!mProperties.mTransparent)
		flags |= BGFX_CLEAR_COLOR;

	bgfx::setViewClear(mProperties.mViewId, flags, mProperties.mClearColor, 1.0f, 0);
}

void eLayer::DestroyFB()
{
	if (bgfx::isValid(mFramebuffer.mFbh))
	{
		if (bgfx::isValid(mFramebuffer.mTexture))
		{
			bgfx::destroy(mFramebuffer.mTexture);
			mFramebuffer.mTexture.idx = bgfx::kInvalidHandle;
		}
		if (bgfx::isValid(mFramebuffer.mDepthMap))
		{
			bgfx::destroy(mFramebuffer.mDepthMap);
			mFramebuffer.mDepthMap.idx = bgfx::kInvalidHandle;
		}
		bgfx::destroy(mFramebuffer.mFbh);
		mFramebuffer.mFbh.idx = bgfx::kInvalidHandle;
		bgfx::setViewFrameBuffer(mProperties.mViewId, BGFX_INVALID_HANDLE);
	}
}

void eLayer::SetFBViewId(bgfx::ViewId viewId)
{
	if (mUseFramebuffer)
	{
		mProperties.mViewId = viewId;
		bgfx::setViewFrameBuffer(mProperties.mViewId, mFramebuffer.mFbh);
	}
}

void eLayer::UpdateFB(vec2i texSize, eTextureFormat format)
{
	if (!bgfx::isValid(mFramebuffer.mFbh) && mUseFramebuffer)
	{
		if (mProperties.mDepthOnly)
		{
			bgfx::TextureHandle fbtextures[] =
			{
				bgfx::createTexture2D(
					  (uint16)texSize.x
					, (uint16)texSize.y
					, false
					, 1
					, bgfx::TextureFormat::D16					//D16 D32F
					, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
					)
			};
			//1: Depth tex

			mFramebuffer.mTexture = BGFX_INVALID_HANDLE;
			mFramebuffer.mDepthMap = fbtextures[0];
			mFramebuffer.mTexSize = texSize;
			mFramebuffer.mFbh = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
		}
		else
		{
			bgfx::TextureHandle fbtextures[] =
			{
				bgfx::createTexture2D(
					  (uint16)texSize.x
					, (uint16)texSize.y
					, false
					, 1
					, format
					, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
					),
				bgfx::createTexture2D(
					  (uint16)texSize.x
					, (uint16)texSize.y
					, false
					, 1
					, bgfx::TextureFormat::D32F					//D16
					, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
					)
			};
			//1: Normal tex; 2: Depth tex

			mFramebuffer.mTexture = fbtextures[0];
			mFramebuffer.mDepthMap = fbtextures[1];
			mFramebuffer.mTexSize = texSize;
			mFramebuffer.mFbh = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
		}
	}
}

void eLayer::SetViewport(eViewport3D* viewport)
{
	mViewport = viewport;
}

eViewport3D* eLayer::GetViewport()
{
	return mViewport;
}

eLayerProperties& eLayer::GetProperties()
{
	return mProperties;
}

eFramebuffer& eLayer::GetFramebuffer()
{
	return mFramebuffer;
}