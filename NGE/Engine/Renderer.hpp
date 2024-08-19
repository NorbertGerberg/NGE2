/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
#include "RenderAPI.hpp"
#include "Shader.hpp"
#include "Layer.hpp"
#include "Graphics.hpp"
#include <vector>

class eWindow;

//! Base for rendering engine
//! \ingroup Engine
class eRenderer : public eBase
{
public:
	eRenderer(HWND window, uint32 width, uint32 height, bool vsync, uint8 msaa, eRenderAPI api);
	~eRenderer();
	virtual void PreUpdate(bool vsync, uint8 msaa) = 0;
	virtual void Update() = 0;

	void SetGfx(eGraphics* gfx);
	eGraphics* GetGfx();

	void PushLayer(eString name, bool useFramebuffer = true);
	void PopLayer(eString name);
	eLayer* GetLayer(eString name);
	std::vector<std::pair<eString, eLayer*>>* GetLayers();

	void PushShader(eString name);
	void PopShader(eString name);
	eShader* GetShader(eString name);
	std::vector<std::pair<eString, eShader*>>* GetShaders();

	vec2i UpdateSize(eWindow* wnd, bool vsync, uint8 msaa);

protected:
	eGraphics* mGfx;
	std::vector<std::pair<eString, eLayer*>>	mLayers;
	std::vector<std::pair<eString, eShader*>>	mShaders;
};
#endif