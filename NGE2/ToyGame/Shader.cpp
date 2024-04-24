/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Shader.hpp"
#include "Debug.hpp"
#include <Console.hpp>
#include "Graphics.hpp"

eShader::~eShader()
{
	for (auto& it : mUniforms)
		eGraphics::Destroy(it.mHandle);
	mUniforms.clear();
	eGraphics::Destroy(mProgram);
}

eString eShader::GetName()
{
	return mName;
}

void eShader::LoadShader(eString name)
{
	mName = name;

	strg shaderPath;
	switch (bgfx::getRendererType())
	{
	case bgfx::RendererType::Direct3D11: {shaderPath = "shaders/d3d/"; } break;
	case bgfx::RendererType::Direct3D12: {shaderPath = "shaders/d3d/"; } break;
	case bgfx::RendererType::Vulkan: {shaderPath = "shaders/spirv/"; } break;
	case bgfx::RendererType::Count: { NGE_ASSERT(false, "Error: No valid API."); } break;
	}

	bx::FileReader vFileReader;
	bx::Error* err = new bx::Error();
	if (!vFileReader.open(strg(shaderPath + name.Get() + ".vsb").c_str(), err))
	{
		eConsole::PrintLog("ERROR::eShader::LoadShader", "Failed to load vertex shader file.");
		eConsole::WriteToDisk();
		return;
	}

	bx::FileReader fFileReader;
	if (!fFileReader.open(strg(shaderPath + name.Get() + ".fsb").c_str(), err))
	{
		eConsole::PrintLog("ERROR::eShader::LoadShader", "Failed to load fragment shader file.");
		eConsole::WriteToDisk();
		return;
	}

	if (err)
		delete err;

	bgfx::ShaderHandle vsh = bgfx::createShader(GetMem(vFileReader));
	bgfx::setName(vsh, strg(name.GetStrg() + "_vs").c_str());

	bgfx::ShaderHandle fsh = bgfx::createShader(GetMem(fFileReader));
	bgfx::setName(vsh, strg(name.GetStrg() + "_fs").c_str());

	mProgram = bgfx::createProgram(vsh, fsh, true);
}

eVertexBufferHandle eShader::CreateVertexBuffer(const void* data, uint32 size, const eVertexLayout& layout)
{
	return bgfx::createVertexBuffer(bgfx::copy(data, size), layout);
}

eIndexBufferHandle eShader::CreateIndexBuffer(const void* data, uint32 size)
{
	return bgfx::createIndexBuffer(bgfx::copy(data, size));
}

void eShader::ApplyVertexBuffer(uint8 stream, const eVertexBufferHandle& handle)
{
	bgfx::setVertexBuffer(stream, handle);
}

void eShader::ApplyIndexBuffer(const eIndexBufferHandle& handle)
{
	bgfx::setIndexBuffer(handle);
}

void eShader::Submit(const bgfx::ViewId& viewID, const bool depth)
{
	bgfx::submit(viewID, mProgram, depth);
	bgfx::discard(0
		BGFX_DISCARD_INDEX_BUFFER
		| BGFX_DISCARD_VERTEX_STREAMS
		| BGFX_DISCARD_BINDINGS
		| BGFX_DISCARD_STATE
		| BGFX_DISCARD_TRANSFORM
	);
}

bgfx::ProgramHandle& eShader::GetProgram()
{
	return mProgram;
}

const bgfx::Memory* eShader::GetMem(bx::FileReader& fileReader)
{
	bx::Error* err = new bx::Error();
	bx::seek(&fileReader, 0, bx::Whence::End);
	uint32 size = uint32(bx::getSize(&fileReader));
	const bgfx::Memory* mem = bgfx::alloc(size + 1);
	bx::seek(&fileReader, 0, bx::Whence::Begin);
	bx::read(&fileReader, mem->data, size, err);
	bx::close(&fileReader);
	mem->data[size] = '\0';
	if (err)
		delete err;
	return mem;
}

void eShader::InitUniform(strg name, eUniformType type, const uint16 nmb)
{
	mUniforms.push_back({ bgfx::createUniform(name.c_str(), type, nmb), name });
}

void eShader::SetUniform(strg name, const void* vl, const uint16 nmb)
{
	for (auto& it : mUniforms)
	{
		if (it.mName.Compare(name))
		{
			bgfx::setUniform(it.mHandle, vl, nmb);
			break;
		}
	}
}

void eShader::SetTexture(uint8 stage, strg name, eTexture* texture)
{
	for (auto& it : mUniforms)
	{
		if (it.mName.Compare(name))
		{
			bgfx::setTexture(stage, it.mHandle, texture->GetHandle());
			break;
		}
	}
}

eUniform* eShader::GetUniform(strg name)
{
	for (auto& it : mUniforms)
	{
		if (it.mName.Compare(name))
			return &it;
	}
	return nullptr;
}