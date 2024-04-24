/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Texture.hpp"
#include <Console.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include <bimg/bimg.h>
#include <bimg/encode.h>

eQuality* eTexture::mQuality = nullptr;

eTexture::eTexture(bool autoDestroy)
{
	mAutoDestroy = autoDestroy;
	mSolidQuality = false;
}

eTexture::~eTexture()
{
	if (mAutoDestroy)
		Release();
}

void eTexture::Release()
{
	if (bgfx::isValid(mHandle))
	{
		bgfx::destroy(mHandle);
		mHandle.idx = bgfx::kInvalidHandle;
	}
}

void eTexture::Load(eString filename, const bool flipUV, uint64 flags)
{
	stbi_set_flip_vertically_on_load(flipUV);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.Get(), &width, &height, &nrComponents, 0);

	if (data)
	{
		unsigned char* out = data;
		if (mQuality != nullptr && !mSolidQuality)
		{
			switch (*mQuality)
			{
			case eQuality::eVeryLow:
			{
				out = nullptr;
				out = new unsigned char[(width / 8) * (height / 8) * nrComponents];
				stbir_resize_uint8(data, width, height, 0, out, width / 8, height / 8, 0, nrComponents);
				width /= 8;
				height /= 8;
				stbi_image_free(data);
			}break;
			case eQuality::eLow:
			{
				out = nullptr;
				out = new unsigned char[(width / 4) * (height / 4) * nrComponents];
				stbir_resize_uint8(data, width, height, 0, out, width / 4, height / 4, 0, nrComponents);
				width /= 4;
				height /= 4;
				stbi_image_free(data);
			}break;
			case eQuality::eMedium:
			{
				out = nullptr;
				out = new unsigned char[(width / 2) * (height / 2) * nrComponents];
				stbir_resize_uint8(data, width, height, 0, out, width / 2, height / 2, 0, nrComponents);
				width /= 2;
				height /= 2;
				stbi_image_free(data);
			}break;
			}
		}

		bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R8;
		if (nrComponents == 3)
			format = bgfx::TextureFormat::RGB8;
		else if (nrComponents == 4)
			format = bgfx::TextureFormat::RGBA8;

		uint64 texFlags = 0;
		uint64 uvwFlag = flags / 1000;
		uint64 borderFlag = (flags % 1000) / 100;
		uint64 compFlag = (flags % 100) / 10;
		uint64 minMaxFlag = flags % 10;

		switch (uvwFlag)
		{
		case 0: {texFlags |= BGFX_SAMPLER_UVW_MIRROR; }break;
		case 1: {texFlags |= BGFX_SAMPLER_UVW_BORDER; }break;
		case 2: {texFlags |= BGFX_SAMPLER_UVW_CLAMP; }break;
		};

		switch (borderFlag)
		{
		case 1: {texFlags |= BGFX_SAMPLER_BORDER_COLOR_SHIFT; }break;
		case 2: {texFlags |= BGFX_SAMPLER_BORDER_COLOR_MASK; }break;
		};

		switch (compFlag)
		{
		case 0: {texFlags |= BGFX_SAMPLER_COMPARE_GREATER; }break;
		case 1: {texFlags |= BGFX_SAMPLER_COMPARE_NOTEQUAL; }break;
		case 2: {texFlags |= BGFX_SAMPLER_COMPARE_NEVER; }break;
		case 3: {texFlags |= BGFX_SAMPLER_COMPARE_ALWAYS; }break;
		case 4: {texFlags |= BGFX_SAMPLER_COMPARE_SHIFT; }break;
		};

		switch (minMaxFlag)
		{
		case 0: {texFlags |= (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT); }break;
		case 1: {texFlags |= (BGFX_SAMPLER_MIN_MASK | BGFX_SAMPLER_MAG_MASK); }break;
		case 2: {texFlags |= (BGFX_SAMPLER_MIN_SHIFT | BGFX_SAMPLER_MAG_SHIFT); }break;
		case 3: {texFlags |= (BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC); }break;
		};

		mHandle = bgfx::createTexture2D(
			static_cast<uint16>(width),
			static_cast<uint16>(height),
			false,
			0,
			format,
			texFlags,
			bgfx::copy(out, width * height * nrComponents));

		if (bgfx::isValid(mHandle))
			bgfx::setName(mHandle, filename.Get());

		mSize = vec2i(width, height);

		stbi_image_free(out);
	}
	else
	{
		stbi_image_free(data);
		eConsole::PrintLog("WARNING::eTexture::Load", "Failed to load texture '" + filename.GetStrg() + "'!");
		eConsole::WriteToDisk();
	}
	stbi_set_flip_vertically_on_load(false);
}

void eTexture::LoadMem(eTextureData& data, uint64 flags)
{
	if (data.mData)
	{
		bgfx::TextureFormat::Enum format = bgfx::TextureFormat::R8;
		if (data.mNbComponents == 3)
			format = bgfx::TextureFormat::RGB8;
		else if (data.mNbComponents == 4)
			format = bgfx::TextureFormat::RGBA8;

		uint64 texFlags = 0;
		uint64 uvwFlag = flags / 1000;
		uint64 borderFlag = (flags % 1000) / 100;
		uint64 compFlag = (flags % 100) / 10;
		uint64 minMaxFlag = flags % 10;

		switch (uvwFlag)
		{
		case 0: {texFlags |= BGFX_SAMPLER_UVW_MIRROR; }break;
		case 1: {texFlags |= BGFX_SAMPLER_UVW_BORDER; }break;
		case 2: {texFlags |= BGFX_SAMPLER_UVW_CLAMP; }break;
		};

		switch (borderFlag)
		{
		case 1: {texFlags |= BGFX_SAMPLER_BORDER_COLOR_SHIFT; }break;
		case 2: {texFlags |= BGFX_SAMPLER_BORDER_COLOR_MASK; }break;
		};

		switch (compFlag)
		{
		case 0: {texFlags |= BGFX_SAMPLER_COMPARE_GREATER; }break;
		case 1: {texFlags |= BGFX_SAMPLER_COMPARE_NOTEQUAL; }break;
		case 2: {texFlags |= BGFX_SAMPLER_COMPARE_NEVER; }break;
		case 3: {texFlags |= BGFX_SAMPLER_COMPARE_ALWAYS; }break;
		case 4: {texFlags |= BGFX_SAMPLER_COMPARE_SHIFT; }break;
		};

		switch (minMaxFlag)
		{
		case 0: {texFlags |= (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT); }break;
		case 1: {texFlags |= (BGFX_SAMPLER_MIN_MASK | BGFX_SAMPLER_MAG_MASK); }break;
		case 2: {texFlags |= (BGFX_SAMPLER_MIN_SHIFT | BGFX_SAMPLER_MAG_SHIFT); }break;
		case 3: {texFlags |= (BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC); }break;
		};

		mHandle = bgfx::createTexture2D(
			static_cast<uint16>(data.mSize.x),
			static_cast<uint16>(data.mSize.y),
			false,
			0,
			format,
			texFlags,
			bgfx::copy(data.mData, data.mSize.x * data.mSize.y * data.mNbComponents));

		if (bgfx::isValid(mHandle))
			bgfx::setName(mHandle, data.mName.Get());

		mSize = vec2i(data.mSize.x, data.mSize.y);
	}
	else
	{
		eConsole::PrintLog("WARNING::eTexture::LoadMem", "data was invalid!");
		eConsole::WriteToDisk();
	}
}

void eTexture::SetHandle(eTextureHandle handle)
{
	mHandle = handle;
}

eTextureHandle eTexture::GetHandle()
{
	return mHandle;
}

vec2i eTexture::GetSize()
{
	return mSize;
}