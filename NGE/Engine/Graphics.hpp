/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
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

typedef bgfx::Caps eGPU_INFO;

//! Simple vertex layout for a 2D quad
//! \ingroup Engine
struct eQuadVertex
{
	real3 x;
	real3 y;
	real3 u;
	real3 v;
};

//! Shader message callback
//! \ingroup Engine
struct eShaderMsgBack
{
	strg mMsg;
	strg mMsg2;
};

//! Instance data buffer
//! \ingroup Engine
struct eIDB
{
	bgfx::InstanceDataBuffer	mBuffer;
	int							mCouldNotDraw;
};

//! Atlas instance data
//! \ingroup Engine
struct eAtlasInstance
{
	eTransformation2D	mTransformation;
	vec2				mSubImg;
};

//! Screen scissor info
//! \ingroup Engine
struct eScissorInfo
{
	vec2i mPosition;
	vec2i mSize;
};

//! Graphics class
//! \ingroup Engine
class eGraphics : public eBase
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

	void InitIDB(eLayer* layer, std::vector<eTransformation2D>& transformations, eIDB& idb);
	void InitIDBAtlas(eLayer* layer, std::vector<eAtlasInstance>& instances, eIDB& idb);

	void Draw2D(eShader* shader, eLayer* layer, eViewport2D vp, eTransformation2D transformation, eTexture* texture);
	const int Draw2DInstances(eShader* shader, eLayer* layer, eViewport2D vp, std::vector<eTransformation2D>& transformations, eTexture* texture);
	void Draw2DInstances(eShader* shader, eLayer* layer, eViewport2D vp, eIDB& idb, eTexture* texture);
	void Draw2DAtlas(eShader* shader, eLayer* layer, eViewport2D vp, eTransformation2D transformation, eTextureAtlas& atlas, vec2 subImg);
	const int Draw2DAtlasInstances(eShader* shader, eLayer* layer, eViewport2D vp, std::vector<eTransformation2D>& transformations, eTextureAtlas& atlas, vec2 subImg);
	const int Draw2DAtlasInstances(eShader* shader, eLayer* layer, eViewport2D vp, std::vector<eAtlasInstance>& instances, eTextureAtlas& atlas);
	void Draw2DAtlasInstances(eShader* shader, eLayer* layer, eViewport2D vp, eIDB& idb, eTextureAtlas& atlas);
	void Draw2DAtlasInstances(eShader* shader, eLayer* layer, eViewport2D vp, eIDB& idb, eTextureAtlas& atlas, vec2 subImg);
	void ResetAtlasShader(eShader* shader);
	void Draw3D(eShader* shader, eLayer* layer, eTransformation3D transformation, eTexture* texture);
	void DrawBillboard(eShader* shader, eLayer* layer, eTransformation3D transformation, eTexture* texture);
	void DrawString(eShader* shader, eLayer* layer, eViewport2D vp, eTransformation2D transformation, eTextAtlas& atlas, eString text, eColor4 color = eColor4(1.0f));

	bool CompileShader(eString name);

	const eGPU_INFO* GetGPUInfo();

	eScissorInfo mScissorInfo;
	void ApplyScissor();
	void ResetScissor();

	static eString mDevShaderDir;

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

	bool bEnableDockingDI;

	bool CompileShaderFA(eString name);
	eShaderMsgBack CompileShaderSingle(eString name, bool vk = false);
	strg CompileShaderTask(strg arguments);

	const int DrawInsBegin(eLayer* layer, std::vector<eTransformation2D>& transformations, bgfx::InstanceDataBuffer& idb);
	const int DrawInsBeginAtlas(eLayer* layer, std::vector<eAtlasInstance>& instances, bgfx::InstanceDataBuffer& idb);
};
#endif