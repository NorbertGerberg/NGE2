/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Renderer.hpp"
#include <Console.hpp>
#include "Window.hpp"

eRenderer::eRenderer(HWND window, uint32 width, uint32 height, bool vsync, uint8 msaa, eRenderAPI api)
{
	mGfx = new eGraphics(window, width, height, vsync, msaa, api);
}

eRenderer::~eRenderer()
{
	for (auto& it : mShaders)
	{
		if (it.second != nullptr)
			delete it.second;
		it.first.Clear();
	}
	mShaders.clear();

	for (auto& it : mLayers)
	{
		if (it.second != nullptr)
			delete it.second;
		it.first.Clear();
	}
	mLayers.clear();

	if (mGfx)
		delete mGfx;
}

void eRenderer::SetGfx(eGraphics* gfx)
{
	mGfx = gfx;
}

eGraphics* eRenderer::GetGfx()
{
	return mGfx;
}

void eRenderer::PushLayer(eString name, bool useFramebuffer)
{
	eLayer* rtVl = new eLayer(useFramebuffer);
	mLayers.push_back(std::make_pair(name, rtVl));
	rtVl = nullptr;
}

void eRenderer::PopLayer(eString name)
{
	for (uint i = 0; i < mLayers.size(); i++)
	{
		auto& it = mLayers[i];
		if (it.first.Compare(name))
		{
			if (it.second != nullptr)
				delete it.second;
			it.first.Clear();
			mLayers.erase(mLayers.begin() + i);
			return;
		}
	}
}

eLayer* eRenderer::GetLayer(eString name)
{
	for (auto& it : mLayers)
	{
		if (it.first.Compare(name))
			return it.second;
	}
	return nullptr;
}

std::vector<std::pair<eString, eLayer*>>* eRenderer::GetLayers()
{
	return &mLayers;
}

void eRenderer::PushShader(eString name)
{
	eShader* rtVl = new eShader();
	rtVl->LoadShader(name);
	mShaders.push_back(std::make_pair(name, rtVl));
	rtVl = nullptr;
}

void eRenderer::PopShader(eString name)
{
	for (uint i = 0; i < mShaders.size(); i++)
	{
		auto& it = mShaders[i];
		if (it.first.Compare(name))
		{
			if (it.second != nullptr)
				delete it.second;
			it.first.Clear();
			mShaders.erase(mShaders.begin() + i);
			return;
		}
	}
}

eShader* eRenderer::GetShader(eString name)
{
	for (auto& it : mShaders)
	{
		if (it.first.Compare(name))
			return it.second;
	}
	return nullptr;
}

std::vector<std::pair<eString, eShader*>>* eRenderer::GetShaders()
{
	return &mShaders;
}

vec2i eRenderer::UpdateSize(eWindow* wnd, bool vsync, uint8 msaa)
{
	vec2i size = wnd->GetSize();
	int width = size.x;
	int height = size.y;
	mGfx->Touch(width, height, vsync, msaa);
	wnd->SetSize(vec2i(width, height));
	return vec2i(width, height);
}