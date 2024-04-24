/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef SHADER_HPP
#define SHADER_HPP
#include <Types.hpp>
#include <String.hpp>
#include "Texture.hpp"
#include <bgfx/bgfx.h>
#include <bx/file.h>
#include <vector>

typedef bgfx::UniformType::Enum			eUniformType;
typedef bgfx::VertexBufferHandle		eVertexBufferHandle;
typedef bgfx::DynamicVertexBufferHandle eDynamicVertexBufferHandle;
typedef bgfx::IndexBufferHandle			eIndexBufferHandle;
typedef bgfx::VertexLayout				eVertexLayout;
typedef bgfx::UniformHandle				eUniformHandle;
typedef bgfx::ProgramHandle				eProgramHandle;

struct eUniform
{
	eUniformHandle mHandle;
	eString		   mName;
};

class eShader
{
public:
	~eShader();
	eString GetName();
	void LoadShader(eString name);
	static eVertexBufferHandle CreateVertexBuffer(const void* data, uint32 size, const eVertexLayout& layout);
	static eIndexBufferHandle CreateIndexBuffer(const void* data, uint32 size);
	void ApplyVertexBuffer(uint8 stream, const eVertexBufferHandle& handle);
	void ApplyIndexBuffer(const eIndexBufferHandle& handle);
	void Submit(const bgfx::ViewId& viewID, const bool depth = false);
	bgfx::ProgramHandle& GetProgram();
	void InitUniform(strg name, eUniformType type, const uint16 nmb = 1);
	void SetUniform(strg name, const void* vl, const uint16 nmb = 1);
	void SetTexture(uint8 stage, strg name, eTexture* texture);
	eUniform* GetUniform(strg name);

private:
	eProgramHandle			mProgram;
	std::vector<eUniform>	mUniforms;
	eString					mName;

	const bgfx::Memory* GetMem(bx::FileReader& fileReader);
};
#endif