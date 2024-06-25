/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Graphics.hpp"
#include <Console.hpp>
#include "Tools.hpp"
#include <EDR.hpp>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>

eGraphics::eGraphics(HWND window, uint32 width, uint32 height, bool vsync, uint8 msaa, eRenderAPI api)
{
	mHwnd = window;
	//bgfx::renderFrame();

	mInit.platformData.nwh = window;
	mInit.resolution.width = width;
	mInit.resolution.height = height;
	if (vsync)
		mInit.resolution.reset = BGFX_RESET_VSYNC;

	switch (msaa)
	{
	case 2: mInit.resolution.reset |= BGFX_RESET_MSAA_X2; break;
	case 4: mInit.resolution.reset |= BGFX_RESET_MSAA_X4; break;
	case 8: mInit.resolution.reset |= BGFX_RESET_MSAA_X8; break;
	case 16: mInit.resolution.reset |= BGFX_RESET_MSAA_X16; break;
	}

	switch (api)
	{
	case eRenderAPI::Direct3D11: mInit.type = bgfx::RendererType::Direct3D11; break;
	case eRenderAPI::Direct3D12: mInit.type = bgfx::RendererType::Direct3D12; break;
	case eRenderAPI::Vulkan: mInit.type = bgfx::RendererType::Vulkan; break;
	}

	if (!bgfx::init(mInit))
	{
		eConsole::PrintLog("ERROR::eGraphics", "Failed to initialize bgfx.");
		eConsole::WriteToDisk();
		return;
	}

	bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
	bgfx::reset((uint32)width, (uint32)height, (vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE) | mInit.resolution.reset);
	bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

	Init2D();
	InitBillboard();
}

eGraphics::~eGraphics()
{
	Release2D();
	ReleaseBillboard();
	bgfx::shutdown();
}

void eGraphics::Frame()
{
#if _DEBUG
	//BGFX_DEBUG_STATS | BGFX_DEBUG_TEXT
	bgfx::setDebug(BGFX_DEBUG_TEXT);
#endif
	bgfx::frame();
}

void eGraphics::SetState(uint64 state)
{
	if (state == 0)
		bgfx::setState(0
			BGFX_STATE_DEFAULT
			| BGFX_STATE_BLEND_ALPHA
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA
		);
	else
		bgfx::setState(0 BGFX_STATE_DEFAULT | state);
}

void eGraphics::Printf(uint16 x, uint16 y, uint8 attr, const char* format)
{
#if _DEBUG
	bgfx::dbgTextPrintf(x, y, attr, format);
#endif
	return;
}

void eGraphics::Touch(int& width, int& height, bool vsync, uint8 msaaSetting)
{
	int oldWidth = width, oldHeight = height;

	RECT wr;
	GetClientRect(mHwnd, &wr);
	width = wr.right - wr.left;
	height = wr.bottom - wr.top;

	if (width != oldWidth || height != oldHeight)
	{
		eLayer::mRefreshSize = true;
		uint32 msaaFlag = BGFX_RESET_NONE;
		switch (msaaSetting)
		{
		case 2: msaaFlag = BGFX_RESET_MSAA_X2; break;
		case 4: msaaFlag = BGFX_RESET_MSAA_X4; break;
		case 8: msaaFlag = BGFX_RESET_MSAA_X8; break;
		case 16: msaaFlag = BGFX_RESET_MSAA_X16; break;
		}
		bgfx::reset((uint32)width, (uint32)height, (vsync ? BGFX_RESET_VSYNC : BGFX_RESET_NONE) | msaaFlag);
		bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
	}
	else
		eLayer::mRefreshSize = false;

	bgfx::touch(0);
#if _DEBUG
	bgfx::dbgTextClear();
#endif
}

void eGraphics::InitIDB(eLayer* layer, std::vector<eTransformation2D>& transformations, eIDB& idb)
{
	idb.mCouldNotDraw = DrawInsBegin(layer, transformations, idb.mBuffer);
}

void eGraphics::Draw2D(eShader* shader, eLayer* layer, eViewport2D vp, eTransformation2D transformation, eTexture* texture)
{
	if (texture == nullptr || shader == nullptr || layer == nullptr)
		return;

	real3 proj[16];
	bx::mtxOrtho(proj, vp.GetTranslation().x, vp.GetSize().x + vp.GetTranslation().x,
		vp.GetSize().y + vp.GetTranslation().y, vp.GetTranslation().y, 0.1f, 100.0f, 0.0f, false);
	bgfx::setViewTransform(layer->GetProperties().mViewId, mView2D, proj);

	real3 posX = (layer->GetProperties().mResolution.x / transformation.mScale.x) / (layer->GetProperties().mResolution.x / transformation.mPosition.x);
	real3 posY = (layer->GetProperties().mResolution.y / transformation.mScale.y) / (layer->GetProperties().mResolution.y / transformation.mPosition.y);

	vec3 rotPiv = vec3(transformation.mRotationPivot.x, transformation.mRotationPivot.y, 0.0f);
	mat4 mdl = eMath::scale(mat4(1.0f), vec3(transformation.mScale.x, transformation.mScale.y, 1.0f));
	mdl = eMath::translate(mdl, vec3(posX, posY, 0.0f) + rotPiv);
	mdl = eMath::rotate(mdl, glm::radians(transformation.mRotation), vec3(0.0f, 0.0f, 1.0f));
	mdl = eMath::translate(mdl, -rotPiv);

	bgfx::setTransform(eMath::value_ptr(mdl));
	SetState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);

	shader->ApplyVertexBuffer(0, mVbh2D);
	shader->SetTexture(0, "s_texColor", texture);
	shader->Submit(layer->GetProperties().mViewId, true);
}

const int eGraphics::Draw2DInstances(eShader* shader, eLayer* layer, eViewport2D vp, std::vector<eTransformation2D>& transformations, eTexture* texture)
{
	if (texture == nullptr || shader == nullptr || layer == nullptr)
		return -1;

	real3 proj[16];
	bx::mtxOrtho(proj, vp.GetTranslation().x, vp.GetSize().x + vp.GetTranslation().x,
		vp.GetSize().y + vp.GetTranslation().y, vp.GetTranslation().y, 0.1f, 100.0f, 0.0f, false);
	bgfx::setViewTransform(layer->GetProperties().mViewId, mView2D, proj);

	bgfx::InstanceDataBuffer idb;
	const int rtVl = DrawInsBegin(layer, transformations, idb);
	
	SetState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);
	shader->ApplyVertexBuffer(0, mVbh2D);
	bgfx::setInstanceDataBuffer(&idb);
	shader->SetTexture(0, "s_texColor", texture);
	shader->Submit(layer->GetProperties().mViewId, true);

	return rtVl;
}

void eGraphics::Draw2DInstances(eShader* shader, eLayer* layer, eViewport2D vp, eIDB& idb, eTexture* texture)
{
	if (texture == nullptr || shader == nullptr || layer == nullptr)
		return;

	real3 proj[16];
	bx::mtxOrtho(proj, vp.GetTranslation().x, vp.GetSize().x + vp.GetTranslation().x,
		vp.GetSize().y + vp.GetTranslation().y, vp.GetTranslation().y, 0.1f, 100.0f, 0.0f, false);
	bgfx::setViewTransform(layer->GetProperties().mViewId, mView2D, proj);

	SetState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ALPHA);
	shader->ApplyVertexBuffer(0, mVbh2D);
	bgfx::setInstanceDataBuffer(&idb.mBuffer);
	shader->SetTexture(0, "s_texColor", texture);
	shader->Submit(layer->GetProperties().mViewId, true);
}

const int eGraphics::DrawInsBegin(eLayer* layer, std::vector<eTransformation2D>& transformations, bgfx::InstanceDataBuffer& idb)
{
	const int insCnt = transformations.size();
	const uint16 insStride = 64;
	uint32 drawnIns = bgfx::getAvailInstanceDataBuffer(insCnt, insStride);

	const int couldNotDraw = insCnt - drawnIns;

	bgfx::allocInstanceDataBuffer(&idb, drawnIns, insStride);

	uint8* data = idb.data;

	for (uint32 i = 0; i < drawnIns; i++)
	{
		auto& transformation = transformations[i];
		real3 posX = (layer->GetProperties().mResolution.x / transformation.mScale.x) / (layer->GetProperties().mResolution.x / transformation.mPosition.x);
		real3 posY = (layer->GetProperties().mResolution.y / transformation.mScale.y) / (layer->GetProperties().mResolution.y / transformation.mPosition.y);

		vec3 rotPiv = vec3(transformation.mRotationPivot.x, transformation.mRotationPivot.y, 0.0f);
		mat4 mdl = eMath::scale(mat4(1.0f), vec3(transformation.mScale.x, transformation.mScale.y, 1.0f));
		mdl = eMath::translate(mdl, vec3(posX, posY, 0.0f) + rotPiv);
		mdl = eMath::rotate(mdl, glm::radians(transformation.mRotation), vec3(0.0f, 0.0f, 1.0f));
		mdl = eMath::translate(mdl, -rotPiv);

		const float* _mtx = eMath::value_ptr(mdl);
		memcpy(data, _mtx, insStride);
		data += insStride;
	}

	return couldNotDraw;
}

void eGraphics::Draw2DAtlas(eShader* shader, eLayer* layer, eViewport2D vp, eTransformation2D transformation, eTextureAtlas& atlas, vec2 subImg)
{
	vec4 atinf[2];
	atinf[0] = vec4(subImg, atlas.mSize);
	atinf[1] = vec4(atlas.mSubSize, 1.0f, 0.0f);
	shader->SetUniform("atlasInfo", atinf, 2);

	Draw2D(shader, layer, vp, transformation, atlas.mTexture);
}

const int eGraphics::Draw2DAtlasInstances(eShader* shader, eLayer* layer, eViewport2D vp, std::vector<eTransformation2D>& transformations, eTextureAtlas& atlas, vec2 subImg)
{
	vec4 atinf[2];
	atinf[0] = vec4(subImg, atlas.mSize);
	atinf[1] = vec4(atlas.mSubSize, 1.0f, 0.0f);
	shader->SetUniform("atlasInfo", atinf, 2);

	return Draw2DInstances(shader, layer, vp, transformations, atlas.mTexture);
}

void eGraphics::Draw2DAtlasInstances(eShader* shader, eLayer* layer, eViewport2D vp, eIDB& idb, eTextureAtlas& atlas, vec2 subImg)
{
	vec4 atinf[2];
	atinf[0] = vec4(subImg, atlas.mSize);
	atinf[1] = vec4(atlas.mSubSize, 1.0f, 0.0f);
	shader->SetUniform("atlasInfo", atinf, 2);

	Draw2DInstances(shader, layer, vp, idb, atlas.mTexture);
}

void eGraphics::ResetAtlasShader(eShader* shader)
{
	vec4 atinf[2];
	atinf[0] = vec4(0.0f);
	atinf[1] = vec4(0.0f);
	shader->SetUniform("atlasInfo", atinf, 2);
}

void eGraphics::Draw3D(eShader* shader, eLayer* layer, eTransformation3D transformation, eTexture* texture)
{
	if (texture == nullptr || shader == nullptr || layer == nullptr)
		return;

	mat4 mdl = eTools::Transform(transformation);
	bgfx::setTransform(eMath::value_ptr(mdl));
	SetState();
	shader->ApplyVertexBuffer(0, mVbh2D);
	shader->SetTexture(0, "s_texColor", texture);
	shader->Submit(layer->GetProperties().mViewId, true);
}

void eGraphics::DrawBillboard(eShader* shader, eLayer* layer, eTransformation3D transformation, eTexture* texture)
{
	if (texture == nullptr || shader == nullptr || layer == nullptr)
		return;

	mat4 mdl = eTools::Transform(transformation);
	bgfx::setTransform(eMath::value_ptr(mdl));
	SetState();
	shader->ApplyVertexBuffer(0, mVbhBillboard);
	shader->SetTexture(0, "s_texColor", texture);
	shader->SetUniform("scale", glm::value_ptr(vec4(transformation.mScale.x, transformation.mScale.y, 0.0f, 0.0f)));
	shader->Submit(layer->GetProperties().mViewId, true);
}

void eGraphics::InitBillboard()
{
	static eQuadVertex quadVerticesBb[] = {
		{-0.5f, 0.5f, 0.0f, 1.0f},		//Top-left
		{0.5f, -0.5f, 1.0f, 0.0f},		//Bottom-right
		{-0.5f, -0.5f, 0.0f, 0.0f},		//Bottom-left
		{-0.5f, 0.5f, 0.0f, 1.0f},		//Top-left
		{0.5f, 0.5f, 1.0f, 1.0f},		//Top-right
		{0.5f, -0.5f, 1.0f, 0.0f}		//Bottom-right
	};

	eVertexLayout pcvLayout;
	pcvLayout.begin()
		.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();

	mVbhBillboard = eShader::CreateVertexBuffer(quadVerticesBb, sizeof(quadVerticesBb), pcvLayout);
}

void eGraphics::ReleaseBillboard()
{
	Destroy(mVbhBillboard);
}

void eGraphics::DrawText(eShader* shader, eLayer* layer, eViewport2D vp, eTransformation2D transformation,
	eTextAtlas& atlas, eString text)
{
	eTransformation2D trans = transformation;
	strg stext = text.GetStrg();
	for (auto& it : stext)
	{
		vec2 subImg = vec2(0.0f);
		switch (it)
		{
			case 'b': { subImg.x = 1.0f; }break;
			case 'c': { subImg.x = 2.0f; }break;
			case 'd': { subImg.x = 3.0f; }break;
			case 'e': { subImg.x = 4.0f; }break;
			case 'f': { subImg.x = 5.0f; }break;
			case 'g': { subImg.x = 6.0f; }break;
			case 'h': { subImg.x = 7.0f; }break;
			case 'i': { subImg.x = 8.0f; }break;
			case 'j': { subImg.x = 9.0f; }break;
			case 'k': { subImg.x = 10.0f; }break;
			case 'l': { subImg.x = 11.0f; }break;
			case 'm': { subImg.x = 12.0f; }break;
			case 'n': { subImg.x = 13.0f; }break;
			case 'o': { subImg.x = 14.0f; }break;
			case 'p': { subImg.x = 15.0f; }break;
			case 'q': { subImg.x = 16.0f; }break;
			case 'r': { subImg.x = 17.0f; }break;
			case 's': { subImg.x = 18.0f; }break;
			case 't': { subImg.x = 19.0f; }break;
			case 'u': { subImg.x = 20.0f; }break;
			case 'v': { subImg.x = 21.0f; }break;
			case 'w': { subImg.x = 22.0f; }break;
			case 'x': { subImg.x = 23.0f; }break;
			case 'y': { subImg.x = 24.0f; }break;
			case 'z': { subImg.x = 25.0f; }break;
			case 'ü': { subImg.x = 26.0f; }break;
			case 'ä': { subImg.x = 27.0f; }break;
			case 'ö': { subImg.x = 28.0f; }break;
			case 'A': { subImg = vec2(0.0f, 1.0f); }break;
			case 'B': { subImg = vec2(1.0f, 1.0f); }break;
			case 'C': { subImg = vec2(2.0f, 1.0f); }break;
			case 'D': { subImg = vec2(3.0f, 1.0f); }break;
			case 'E': { subImg = vec2(4.0f, 1.0f); }break;
			case 'F': { subImg = vec2(5.0f, 1.0f); }break;
			case 'G': { subImg = vec2(6.0f, 1.0f); }break;
			case 'H': { subImg = vec2(7.0f, 1.0f); }break;
			case 'I': { subImg = vec2(8.0f, 1.0f); }break;
			case 'J': { subImg = vec2(9.0f, 1.0f); }break;
			case 'K': { subImg = vec2(10.0f, 1.0f); }break;
			case 'L': { subImg = vec2(11.0f, 1.0f); }break;
			case 'M': { subImg = vec2(12.0f, 1.0f); }break;
			case 'N': { subImg = vec2(13.0f, 1.0f); }break;
			case 'O': { subImg = vec2(14.0f, 1.0f); }break;
			case 'P': { subImg = vec2(15.0f, 1.0f); }break;
			case 'Q': { subImg = vec2(16.0f, 1.0f); }break;
			case 'R': { subImg = vec2(17.0f, 1.0f); }break;
			case 'S': { subImg = vec2(18.0f, 1.0f); }break;
			case 'T': { subImg = vec2(19.0f, 1.0f); }break;
			case 'U': { subImg = vec2(20.0f, 1.0f); }break;
			case 'V': { subImg = vec2(21.0f, 1.0f); }break;
			case 'W': { subImg = vec2(22.0f, 1.0f); }break;
			case 'X': { subImg = vec2(23.0f, 1.0f); }break;
			case 'Y': { subImg = vec2(24.0f, 1.0f); }break;
			case 'Z': { subImg = vec2(25.0f, 1.0f); }break;
			case 'Ü': { subImg = vec2(26.0f, 1.0f); }break;
			case 'Ä': { subImg = vec2(27.0f, 1.0f); }break;
			case 'Ö': { subImg = vec2(28.0f, 1.0f); }break;
			case '0': { subImg = vec2(0.0f, 2.0f); }break;
			case '1': { subImg = vec2(1.0f, 2.0f); }break;
			case '2': { subImg = vec2(2.0f, 2.0f); }break;
			case '3': { subImg = vec2(3.0f, 2.0f); }break;
			case '4': { subImg = vec2(4.0f, 2.0f); }break;
			case '5': { subImg = vec2(5.0f, 2.0f); }break;
			case '6': { subImg = vec2(6.0f, 2.0f); }break;
			case '7': { subImg = vec2(7.0f, 2.0f); }break;
			case '8': { subImg = vec2(8.0f, 2.0f); }break;
			case '9': { subImg = vec2(9.0f, 2.0f); }break;
			case ',': { subImg = vec2(10.0f, 2.0f); }break;
			case ';': { subImg = vec2(11.0f, 2.0f); }break;
			case '.': { subImg = vec2(12.0f, 2.0f); }break;
			case ':': { subImg = vec2(13.0f, 2.0f); }break;
			case '-': { subImg = vec2(14.0f, 2.0f); }break;
			case '_': { subImg = vec2(15.0f, 2.0f); }break;
			case '`': { subImg = vec2(16.0f, 2.0f); }break;
			case '´': { subImg = vec2(17.0f, 2.0f); }break;
			case '<': { subImg = vec2(18.0f, 2.0f); }break;
			case '>': { subImg = vec2(19.0f, 2.0f); }break;
			case '|': { subImg = vec2(20.0f, 2.0f); }break;
			case '!': { subImg = vec2(21.0f, 2.0f); }break;
			case '"': { subImg = vec2(22.0f, 2.0f); }break;
			case '§': { subImg = vec2(23.0f, 2.0f); }break;
			case '$': { subImg = vec2(24.0f, 2.0f); }break;
			case '%': { subImg = vec2(25.0f, 2.0f); }break;
			case '&': { subImg = vec2(26.0f, 2.0f); }break;
			case '/': { subImg = vec2(27.0f, 2.0f); }break;
			case '(': { subImg = vec2(28.0f, 2.0f); }break;
			case ')': { subImg = vec2(0.0f, 3.0f); }break;
			case '=': { subImg = vec2(1.0f, 3.0f); }break;
			case '?': { subImg = vec2(2.0f, 3.0f); }break;
			case '\\': { subImg = vec2(3.0f, 3.0f); }break;
			case '}': { subImg = vec2(4.0f, 3.0f); }break;
			case '{': { subImg = vec2(5.0f, 3.0f); }break;
			case ']': { subImg = vec2(6.0f, 3.0f); }break;
			case '[': { subImg = vec2(7.0f, 3.0f); }break;
			case '€': { subImg = vec2(8.0f, 3.0f); }break;
			case '@': { subImg = vec2(9.0f, 3.0f); }break;
			case '#': { subImg = vec2(10.0f, 3.0f); }break;
			case '\'': { subImg = vec2(11.0f, 3.0f); }break;
			case '+': { subImg = vec2(12.0f, 3.0f); }break;
			case '~': { subImg = vec2(13.0f, 3.0f); }break;
			case '*': { subImg = vec2(14.0f, 3.0f); }break;
			case '^': { subImg = vec2(15.0f, 3.0f); }break;
			case '°': { subImg = vec2(16.0f, 3.0f); }break;
			case ' ': { trans.mPosition.x += trans.mScale.x * atlas.mSpace.x; continue; }break;
			case '\n':
			{
				trans.mPosition.x = transformation.mPosition.x;
				trans.mPosition.y += trans.mScale.y * atlas.mSpace.y;
				continue;
			}break;
		}
		Draw2DAtlas(shader, layer, vp, trans, atlas.mAtlas, subImg);
		trans.mPosition.x += trans.mScale.x * atlas.mSpace.x;
	}
	ResetAtlasShader(shader);
}

void eGraphics::Init2D()
{
	static eQuadVertex quadVertices[] = {
		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 0.0f, 1.0f, 0.0f}
	};

	eVertexLayout pcvLayout;
	pcvLayout.begin()
		.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();

	mVbh2D = eShader::CreateVertexBuffer(quadVertices, sizeof(quadVertices), pcvLayout);

	const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -5.0f };
	bx::mtxLookAt(mView2D, eye, at);
}

void eGraphics::Release2D()
{
	Destroy(mVbh2D);
}

void eGraphics::InitDI(GLFWwindow* glfwWin, HWND win, int viewId)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	if (mEnableDockingDI)
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 10.5f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.4000000059604645f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 20.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 6.5f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 4.800000190734863f);
	style.FrameRounding = 2.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 13.0f;
	style.ScrollbarRounding = 12.0f;
	style.GrabMinSize = 7.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 1.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = FLT_MAX;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2532188892364502f, 0.2532163560390472f, 0.2532163560390472f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(9.999999974752427e-07f, 9.999899930335232e-07f, 9.999899930335232e-07f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2784313857555389f, 0.2784313857555389f, 0.2784313857555389f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2745098173618317f, 0.2745098173618317f, 0.2745098173618317f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2980392277240753f, 0.2980392277240753f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1560000032186508f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.3910000026226044f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 1.0f, 1.0f, 0.25f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.6700000166893005f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.09411764889955521f, 0.09411764889955521f, 0.09411764889955521f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1921568661928177f, 0.1921568661928177f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.5843137502670288f, 0.5843137502670288f, 0.5843137502670288f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.1560000032186508f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 0.3882353007793427f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5860000252723694f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5860000252723694f);

	if (glfwWin != nullptr)
		ImGui_ImplGlfw_InitForOther(glfwWin, true);
	else
		ImGui_ImplWin32_Init(win);
	ImGui_Implbgfx_Init(viewId);
}

void eGraphics::ReleaseDI(bool glfwWin)
{
	ImGui_Implbgfx_Shutdown();
	if (glfwWin)
		ImGui_ImplGlfw_Shutdown();
	else
		ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void eGraphics::BeginDI(bool glfwWin)
{
	ImGui_Implbgfx_NewFrame();
	if (glfwWin)
		ImGui_ImplGlfw_NewFrame();
	else
		ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (mEnableDockingDI)
	{
		static ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x - 40.0f, viewport->Size.y));
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::SetNextWindowBgAlpha(0.0f);

		//ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, style.FramePadding.y * 2 + ImGui::GetFontSize()));
		bool p_open = true;
		ImGui::Begin("MAINWINDOW", &p_open, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("MAINWINDOW");
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		ImGui::End();
	}
}

void eGraphics::EndDI(bool glfwWin)
{
	ImGui::Render();
	ImGui_Implbgfx_RenderDrawLists(ImGui::GetDrawData());
}

void eGraphics::SetEnableDockingDI(bool vl)
{
	mEnableDockingDI = vl;
}

bool eGraphics::CompileShader(eString name)
{
	if (!EDR::FileExists("shaderc.exe"))
	{
		eConsole::PrintLog("Shader Compiler Error", "'shaderc.exe' could not be found!");
		return false;
	}
	return CompileShaderFA(name);
}

bool eGraphics::CompileShaderFA(eString name)
{
	bool isS = false;
	bShaderMsgBack msg = CompileShaderSingle(name);
	if (msg.mMsg.empty() && msg.mMsg2.empty())
		isS = true;
	else
	{
		strg errMsg;
		if (!msg.mMsg.empty())
		{
			errMsg += "D3D vertex shader error:\n\n";
			errMsg += msg.mMsg + "\n\n\n";
		}

		if (!msg.mMsg2.empty())
		{
			errMsg += "D3D fragment shader error:\n\n";
			errMsg += msg.mMsg2;
		}
		eConsole::PrintLog("Shader Compiler Error", errMsg);
	}

	msg = CompileShaderSingle(name, true);
	if (!msg.mMsg.empty() && !msg.mMsg2.empty())
	{
		isS = false;
		strg errMsg;
		if (!msg.mMsg.empty())
		{
			errMsg += "Spirv vertex shader error:\n\n";
			errMsg += msg.mMsg + "\n\n\n";
		}

		if (!msg.mMsg2.empty())
		{
			errMsg += "Spirv fragment shader error:\n\n";
			errMsg += msg.mMsg2;
		}
		eConsole::PrintLog("Shader Compiler Error", errMsg);
	}
	return isS;
}

bShaderMsgBack eGraphics::CompileShaderSingle(eString name, bool vk)
{
	strg rp = vk ? "spirv" : "d3d";
	strg rcmd = vk ? " -p spirv" : " -p s_5_0 -O 3";

	strg fileName = name.GetStrg();

	strg endFile = "shaders/" + rp + "/" + fileName + ".vsb";
	strg arguments = "-f development/shaders/" + fileName + "/" + fileName + ".vs -o " + endFile + " --type v --platform windows" + rcmd;

	if (EDR::FileExists(endFile.c_str()))
		EDR::RemoveFile(endFile.c_str());

	strg msg = CompileShaderTask(arguments);

	endFile = "shaders/" + rp + "/" + fileName + ".fsb";
	arguments = "-f development/shaders/" + fileName + "/" + fileName + ".fs -o " + endFile + " --type f --platform windows" + rcmd;

	if (EDR::FileExists(endFile.c_str()))
		EDR::RemoveFile(endFile.c_str());

	strg msg2 = CompileShaderTask(arguments);

	bShaderMsgBack rtVl = { msg, msg2 };
	return rtVl;
}

strg eGraphics::CompileShaderTask(strg arguments)
{
	std::ostringstream command;
	command << "shaderc.exe " << arguments;

	FILE* pipe = _popen(command.str().c_str(), "r");
	if (!pipe)
	{
		eConsole::PrintLog("Shader Compiler Error", "Error creating process!");
		return "ERROR";
	}

	std::ostringstream output;
	char buffer[128];
	while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
		output << buffer;

	if (_pclose(pipe) == -1)
	{
		eConsole::PrintLog("Shader Compiler Error", "Error creating process!");
		return "ERROR";
	}

	return output.str();
}

const eGPU_INFO* eGraphics::GetGPUInfo()
{
	return bgfx::getCaps();
}