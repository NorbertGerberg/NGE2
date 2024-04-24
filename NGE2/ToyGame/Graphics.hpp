/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <Types.hpp>
#include <String.hpp>
#include "RenderAPI.hpp"
#include "Transformations.hpp"

#if _DEBUG
#define BX_CONFIG_DEBUG 1 //Enable debug features
#endif
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

#include "Shader.hpp"
#include "Layer.hpp"
#include "Viewport2D.hpp"

#include <imgui_impl_win32.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_bgfx.h>
#define eDI ImGui

#define NGE_INVALID_HANDLE BGFX_INVALID_HANDLE

struct eQuadVertex
{
	real3 x;
	real3 y;
	real3 u;
	real3 v;
};

struct bShaderMsgBack
{
	strg mMsg;
	strg mMsg2;
};

class eGraphics
{
public:
	eGraphics(HWND window, uint32 width, uint32 height, bool vsync, uint8 msaa, eRenderAPI api);
	~eGraphics();

	void Frame();
	static void SetState(uint64 state = 0);
	void Printf(uint16 x, uint16 y, uint8 attr, const char* format);
	void Touch(int& width, int& height, bool vsync, uint8 msaaSetting);

	void InitDI(GLFWwindow* glfwWin, HWND win, int viewId = 1);
	void ReleaseDI(bool glfwWin = true);
	void BeginDI(bool glfwWin = true);
	void EndDI(bool glfwWin = true);
	void SetEnableDockingDI(bool vl = true);

	template<typename var>
	static void Destroy(var& handle)
	{
		if (bgfx::isValid(handle))
		{
			bgfx::destroy(handle);
			handle.idx = bgfx::kInvalidHandle;
		}
	}

	void Draw2D(eShader* shader, eLayer* layer, eViewport2D vp, eTransformation2D transformation, eTexture* texture);
	void Draw2DAtlas(eShader* shader, eLayer* layer, eViewport2D vp, eTransformation2D transformation, eTextureAtlas& atlas, vec2 subImg);
	void ResetAtlasShader(eShader* shader);
	void Draw3D(eShader* shader, eLayer* layer, eTransformation3D transformation, eTexture* texture);
	void DrawBillboard(eShader* shader, eLayer* layer, eTransformation3D transformation, eTexture* texture);

	bool CompileShader(eString name);

private:
	bgfx::Init	mInit;
	HWND		mHwnd;

	void Init2D();
	void Release2D();
	eVertexBufferHandle mVbh2D;
	real3 mView2D[16];

	void InitBillboard();
	void ReleaseBillboard();
	eVertexBufferHandle mVbhBillboard;

	bool mEnableDockingDI;

	bool CompileShaderFA(eString name);
	bShaderMsgBack CompileShaderSingle(eString name, bool vk = false);
	strg CompileShaderTask(strg arguments);
};
#endif