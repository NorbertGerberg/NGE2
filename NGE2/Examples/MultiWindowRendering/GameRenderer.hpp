/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef GAMERENDERER_HPP
#define GAMERENDERER_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Renderer.hpp>

class GameRenderer : public eRenderer
{
public:
	GameRenderer(eWindow* window, uint32 width, uint32 height, bool vsync, uint8 msaa, eRenderAPI api, eWindow* window2);
	~GameRenderer();
	void PreUpdate(bool vsync, uint8 msaa);
	void Update();

	void SetWindow(eWindow* window);
	eWindow* GetWindow();

private:
	eWindow* mWnd;
	eLayer* m2DPass;
	//----------------------------------------------------Multi Window Rendering
	eWindow* mWnd2;
	eLayer* m2DPass2;
	//----------------------------------------------------End
};
#endif