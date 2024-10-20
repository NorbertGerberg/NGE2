/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Texture.hpp"
#include <Console.hpp>
#include <EDR.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include <bimg/bimg.h>
#include <bimg/encode.h>

#include <fstream>
#include <vector>
#include <sstream>

eTexture::eTexture(bool autoDestroy)
{
	bAutoDestroy = autoDestroy;
}

eTexture::~eTexture()
{
	if (bAutoDestroy)
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
	const bool isDXT = filename.Find(".dds");

	if (isDXT)
	{
		std::ifstream file(filename.GetStrg(), std::ios::binary);
		if (!file)
		{
			eConsole::PrintLog("WARNING::eTexture::Load", "Failed to load texture '" + filename.GetStrg() + "'!");
			eConsole::WriteToDisk();
			return;
		}

		file.seekg(0, std::ios::end);
		uint64 size = file.tellg();
		file.seekg(0, std::ios::beg);

		std::vector<uint8> buffer(size);
		file.read(reinterpret_cast<char*>(buffer.data()), size);

		uint64 texFlags = 0;
		uint64 uvwFlag = flags / 1000;
		uint64 borderFlag = (flags % 1000) / 100;
		uint64 compFlag = (flags % 100) / 10;
		uint64 minMaxFlag = flags % 10;

		switch (uvwFlag)
		{
		case 0: { texFlags |= BGFX_SAMPLER_UVW_MIRROR; }break;
		case 1: { texFlags |= BGFX_SAMPLER_UVW_BORDER; }break;
		case 2: { texFlags |= BGFX_SAMPLER_UVW_CLAMP; }break;
		};

		switch (borderFlag)
		{
		case 1: { texFlags |= BGFX_SAMPLER_BORDER_COLOR_SHIFT; }break;
		case 2: { texFlags |= BGFX_SAMPLER_BORDER_COLOR_MASK; }break;
		};

		switch (compFlag)
		{
		case 0: { texFlags |= BGFX_SAMPLER_COMPARE_GREATER; }break;
		case 1: { texFlags |= BGFX_SAMPLER_COMPARE_NOTEQUAL; }break;
		case 2: { texFlags |= BGFX_SAMPLER_COMPARE_NEVER; }break;
		case 3: { texFlags |= BGFX_SAMPLER_COMPARE_ALWAYS; }break;
		case 4: { texFlags |= BGFX_SAMPLER_COMPARE_SHIFT; }break;
		};

		switch (minMaxFlag)
		{
		case 0: { texFlags |= (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT); }break;
		case 1: { texFlags |= (BGFX_SAMPLER_MIN_MASK | BGFX_SAMPLER_MAG_MASK); }break;
		case 2: { texFlags |= (BGFX_SAMPLER_MIN_SHIFT | BGFX_SAMPLER_MAG_SHIFT); }break;
		case 3: { texFlags |= (BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC); }break;
		};

		mHandle = bgfx::createTexture(bgfx::copy(buffer.data(), buffer.size()), texFlags);

		if (bgfx::isValid(mHandle))
			bgfx::setName(mHandle, filename.Get());

		file.seekg(12);
		int width, height;
		file.read(reinterpret_cast<char*>(&height), sizeof(uint32));
		file.read(reinterpret_cast<char*>(&width), sizeof(uint32));
		file.close();
		mSize = vec2i(width, height);
	}
	else
	{
		stbi_set_flip_vertically_on_load(flipUV);
		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.Get(), &width, &height, &nrComponents, 0);

		if (data)
		{
			unsigned char* out = data;
			/*
			if (mQuality != nullptr && !bSolidQuality)
			{
				switch (*mQuality)
				{
				case eQuality::VeryLow:
				{
					out = nullptr;
					out = new unsigned char[(width / 8) * (height / 8) * nrComponents];
					stbir_resize_uint8(data, width, height, 0, out, width / 8, height / 8, 0, nrComponents);
					width /= 8;
					height /= 8;
					stbi_image_free(data);
				}break;
				case eQuality::Low:
				{
					out = nullptr;
					out = new unsigned char[(width / 4) * (height / 4) * nrComponents];
					stbir_resize_uint8(data, width, height, 0, out, width / 4, height / 4, 0, nrComponents);
					width /= 4;
					height /= 4;
					stbi_image_free(data);
				}break;
				case eQuality::Medium:
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
			*/

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
			case 0: { texFlags |= BGFX_SAMPLER_UVW_MIRROR; }break;
			case 1: { texFlags |= BGFX_SAMPLER_UVW_BORDER; }break;
			case 2: { texFlags |= BGFX_SAMPLER_UVW_CLAMP; }break;
			};

			switch (borderFlag)
			{
			case 1: { texFlags |= BGFX_SAMPLER_BORDER_COLOR_SHIFT; }break;
			case 2: { texFlags |= BGFX_SAMPLER_BORDER_COLOR_MASK; }break;
			};

			switch (compFlag)
			{
			case 0: { texFlags |= BGFX_SAMPLER_COMPARE_GREATER; }break;
			case 1: { texFlags |= BGFX_SAMPLER_COMPARE_NOTEQUAL; }break;
			case 2: { texFlags |= BGFX_SAMPLER_COMPARE_NEVER; }break;
			case 3: { texFlags |= BGFX_SAMPLER_COMPARE_ALWAYS; }break;
			case 4: { texFlags |= BGFX_SAMPLER_COMPARE_SHIFT; }break;
			};

			switch (minMaxFlag)
			{
			case 0: { texFlags |= (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT); }break;
			case 1: { texFlags |= (BGFX_SAMPLER_MIN_MASK | BGFX_SAMPLER_MAG_MASK); }break;
			case 2: { texFlags |= (BGFX_SAMPLER_MIN_SHIFT | BGFX_SAMPLER_MAG_SHIFT); }break;
			case 3: { texFlags |= (BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC); }break;
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
}

void eTexture::LoadMem(strg& mem, eString name, uint64 flags)
{
	if (!mem.empty())
	{
		uint64 texFlags = 0;
		uint64 uvwFlag = flags / 1000;
		uint64 borderFlag = (flags % 1000) / 100;
		uint64 compFlag = (flags % 100) / 10;
		uint64 minMaxFlag = flags % 10;

		switch (uvwFlag)
		{
		case 0: { texFlags |= BGFX_SAMPLER_UVW_MIRROR; }break;
		case 1: { texFlags |= BGFX_SAMPLER_UVW_BORDER; }break;
		case 2: { texFlags |= BGFX_SAMPLER_UVW_CLAMP; }break;
		};

		switch (borderFlag)
		{
		case 1: { texFlags |= BGFX_SAMPLER_BORDER_COLOR_SHIFT; }break;
		case 2: { texFlags |= BGFX_SAMPLER_BORDER_COLOR_MASK; }break;
		};

		switch (compFlag)
		{
		case 0: { texFlags |= BGFX_SAMPLER_COMPARE_GREATER; }break;
		case 1: { texFlags |= BGFX_SAMPLER_COMPARE_NOTEQUAL; }break;
		case 2: { texFlags |= BGFX_SAMPLER_COMPARE_NEVER; }break;
		case 3: { texFlags |= BGFX_SAMPLER_COMPARE_ALWAYS; }break;
		case 4: { texFlags |= BGFX_SAMPLER_COMPARE_SHIFT; }break;
		};

		switch (minMaxFlag)
		{
		case 0: { texFlags |= (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT); }break;
		case 1: { texFlags |= (BGFX_SAMPLER_MIN_MASK | BGFX_SAMPLER_MAG_MASK); }break;
		case 2: { texFlags |= (BGFX_SAMPLER_MIN_SHIFT | BGFX_SAMPLER_MAG_SHIFT); }break;
		case 3: { texFlags |= (BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC); }break;
		};

		std::vector<uint8_t> buffer(mem.size());
		std::memcpy(buffer.data(), mem.data(), buffer.size());

		mHandle = bgfx::createTexture(bgfx::copy(buffer.data(), buffer.size()), texFlags);

		if (bgfx::isValid(mHandle))
			bgfx::setName(mHandle, name.Get());

		const char* header = mem.data();
		int height = *reinterpret_cast<const uint32_t*>(header + 12);
		int width = *reinterpret_cast<const uint32_t*>(header + 16);
		mSize = vec2i(width, height);
	}
	else
	{
		eConsole::PrintLog("WARNING::eTexture::LoadMem", "data was invalid!");
		eConsole::WriteToDisk();
	}
}

void eTexture::LoadDXT(strg& mem, eString name, uint64 flags)
{
	if (!mem.empty())
	{
		uint64 texFlags = 0;
		uint64 uvwFlag = flags / 1000;
		uint64 borderFlag = (flags % 1000) / 100;
		uint64 compFlag = (flags % 100) / 10;
		uint64 minMaxFlag = flags % 10;

		switch (uvwFlag)
		{
		case 0: { texFlags |= BGFX_SAMPLER_UVW_MIRROR; }break;
		case 1: { texFlags |= BGFX_SAMPLER_UVW_BORDER; }break;
		case 2: { texFlags |= BGFX_SAMPLER_UVW_CLAMP; }break;
		};

		switch (borderFlag)
		{
		case 1: { texFlags |= BGFX_SAMPLER_BORDER_COLOR_SHIFT; }break;
		case 2: { texFlags |= BGFX_SAMPLER_BORDER_COLOR_MASK; }break;
		};

		switch (compFlag)
		{
		case 0: { texFlags |= BGFX_SAMPLER_COMPARE_GREATER; }break;
		case 1: { texFlags |= BGFX_SAMPLER_COMPARE_NOTEQUAL; }break;
		case 2: { texFlags |= BGFX_SAMPLER_COMPARE_NEVER; }break;
		case 3: { texFlags |= BGFX_SAMPLER_COMPARE_ALWAYS; }break;
		case 4: { texFlags |= BGFX_SAMPLER_COMPARE_SHIFT; }break;
		};

		switch (minMaxFlag)
		{
		case 0: { texFlags |= (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT); }break;
		case 1: { texFlags |= (BGFX_SAMPLER_MIN_MASK | BGFX_SAMPLER_MAG_MASK); }break;
		case 2: { texFlags |= (BGFX_SAMPLER_MIN_SHIFT | BGFX_SAMPLER_MAG_SHIFT); }break;
		case 3: { texFlags |= (BGFX_SAMPLER_MIN_ANISOTROPIC | BGFX_SAMPLER_MAG_ANISOTROPIC); }break;
		};

		std::vector<uint8_t> buffer(mem.size());
		std::memcpy(buffer.data(), mem.data(), buffer.size());

		mHandle = bgfx::createTexture(bgfx::copy(buffer.data(), buffer.size()), texFlags);

		if (bgfx::isValid(mHandle))
			bgfx::setName(mHandle, name.Get());

		const char* header = mem.data();
		int height = *reinterpret_cast<const uint32_t*>(header + 12);
		int width = *reinterpret_cast<const uint32_t*>(header + 16);
		mSize = vec2i(width, height);
	}
	else
	{
		eConsole::PrintLog("WARNING::eTexture::LoadDXT", "data was invalid!");
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

eString eTexture::CompileDXT(eString inputFile, eString outputFile, uint8 bc, const bool mips, const bool isNormalMap)
{
	if (!EDR::FileExists(inputFile.Get()))
	{
		eConsole::PrintLog("ERROR::eTexture::CompileDXT", "Input file could not be found!");
		return "ERROR";
	}

	if (bc == 0 || bc > 5)
	{
		eConsole::PrintLog("ERROR::eTexture::CompileDXT", "Invalid BC format!");
		return "ERROR";
	}
	eString outcmd = eTEXT("texturec.exe -f \"") + inputFile + eTEXT("\" -o \"") + outputFile + eTEXT("\" -t bc") + eString::ToString(bc);

	if (isNormalMap) outcmd += " -n";
	if(mips) outcmd += " -m";

	if (EDR::FileExists(outputFile.Get()))
		EDR::RemoveFile(outputFile.Get());

	std::ostringstream command;
	command << outcmd.GetStrg();

	FILE* pipe = _popen(command.str().c_str(), "r");
	if (!pipe)
	{
		eConsole::PrintLog("ERROR::eTexture::CompileDXT", "Error creating process!");
		return "ERROR";
	}

	std::ostringstream output;
	char buffer[128];
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		output << buffer;

	if (_pclose(pipe) == -1)
	{
		eConsole::PrintLog("ERROR::eTexture::CompileDXT", "Error closing process!");
		return "ERROR";
	}

	return output.str();
}