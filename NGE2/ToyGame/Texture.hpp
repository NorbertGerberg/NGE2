/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Quality.hpp>
#include <bgfx/bgfx.h>

typedef bgfx::TextureInfo			eTextureInfo;
typedef bgfx::TextureFormat::Enum	eTextureFormat;
typedef bgfx::TextureHandle			eTextureHandle;

class eTexture;

struct eTextureAtlas
{
	vec2 mSize = vec2(352.0f);
	vec2 mSubSize = vec2(32.0f);
	eTexture* mTexture = nullptr;
};

struct eTextAtlas
{
	eTextureAtlas	mAtlas;
	vec2			mSpace;
};

struct eTextureData
{
	unsigned char*  mData;
	eString			mName;
	vec2i			mSize;
	int				mNbComponents;
};

/*
Texture flags:
	xyzw
	x:
		0 -> Mirror
		1 -> Border
		2 -> Clamp
	y:
		0 -> border color shift
		1 -> border color mask
	z:
		0 -> compare greater
		1 -> compare not equal
		2 -> compare never
		3 -> compare always
		4 -> compare shift
	w:
		0 -> min/mag point
		1 -> min/mag mask
		2 -> min/mag shift
		3 -> min/mag anisotropic
*/

class eTexture
{
public:
	eTexture(bool autoDestroy = true);
	~eTexture();
	void Release();
	void Load(eString filename, const bool flipUV = true, uint64 flags = 0);
	void LoadMem(eTextureData& data, uint64 flags = 0);
	void SetHandle(eTextureHandle handle);
	eTextureHandle GetHandle();
	vec2i GetSize();

	static eQuality* mQuality;

	bool mSolidQuality;

private:
	eTextureHandle	mHandle = BGFX_INVALID_HANDLE;
	bool			mAutoDestroy;
	vec2i			mSize;
};
#endif