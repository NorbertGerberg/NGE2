/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/GameRenderer.hpp"
#include "GameTask.hpp"
#include <System.hpp>

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 640;
	uint32 height = 520; //300

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("System Info Example");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();
	mWnd->SetWindowResizable(false);

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	mQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);

	mTexture = new eTexture();
	mTexture->Load("tex_font_default.bmp", false, 20);

	//----------------------------------------------------System Info

	mMsg =  "CPUs:                      " + std::to_string(eSystem::GetProcessorCount());
	mMsg += "\nRAM:                       " + std::to_string(eSystem::GetRAMsize()) + " mb";
	mMsg += "\nAvailable RAM:             " + std::to_string(eSystem::GetAvailableRAMsize()) + " mb";
	mMsg += "\nVirtual RAM:               " + std::to_string(eSystem::GetVirtualRAMsize()) + " mb";
	mMsg += "\nAvailable Virtual RAM:     " + std::to_string(eSystem::GetVirtualRAMsize()) + " mb";
	mMsg += "\n\nSpace On Disk:             " + std::to_string(eSystem::GetSpaceOnDisk()) + " mb";
	mMsg += "\nAvailable Space On Disk:   " + std::to_string(eSystem::GetAvailableSpaceOnDisk()) + " mb";

	const eGPU_INFO* gpu = mRenderer->GetGfx()->GetGPUInfo();

	strg vname = "OTHER";
	if (gpu->vendorId == BGFX_PCI_ID_NVIDIA)
		vname = "NVIDIA";
	else if (gpu->vendorId == BGFX_PCI_ID_AMD)
		vname = "AMD";
	else if (gpu->vendorId == BGFX_PCI_ID_INTEL)
		vname = "INTEL";
	else if (gpu->vendorId == BGFX_PCI_ID_ARM)
		vname = "ARM";
	else if (gpu->vendorId == BGFX_PCI_ID_MICROSOFT)
		vname = "MICROSOFT";

	mMsg += "\n\nGPU Vendor:                " + vname;
	mMsg += "\nGPU Amount:                " + std::to_string(gpu->numGPUs);
	mMsg += "\nmax Draw Calls:            " + std::to_string(gpu->limits.maxDrawCalls);
	mMsg += "\nmax Shaders:               " + std::to_string(gpu->limits.maxShaders);
	mMsg += "\nmax Frame Buffers:         " + std::to_string(gpu->limits.maxFrameBuffers);
	mMsg += "\nmax Index Buffers:         " + std::to_string(gpu->limits.maxIndexBuffers);

	//----------------------------------------------------End

	return true;
}

void GameTask::Update()
{
	mTime->Update();
	mQuit = mWnd->WindowCloseRequested();

	UpdateFixedTime();
	while (mAccumulatedTime >= mFixedTime)
	{
		//	Fixed timestep
		//	Update code here!
		PostUpdateFixedTime();
	}
}

void GameTask::Render()
{
	if (!mWnd->IsWindowIconified())
	{
		mRenderer->PreUpdate(true, 16);

		eGraphics* gfx	= mRenderer->GetGfx();
		eShader* shader = mRenderer->GetShader("Text2D");
		eLayer* layer	= mRenderer->GetLayer("2D");

		eTextureAtlas atlas;
		atlas.mSize		= vec2(319.0f, 64.0f);
		atlas.mSubSize	= vec2(11.0f, 16.0f);
		atlas.mTexture	= mTexture;

		eTextAtlas textAtlas;
		textAtlas.mAtlas = atlas;
		textAtlas.mSpace = vec2(0.7f, 1.0f);

		eViewport2D vp2d;
		vp2d.SetTranslation(vec2(0.0f));
		vp2d.SetSize(mWnd->GetWindowSize());
		real3 py = vp2d.GetSize().y - 128.0f;

		eTransformation2D trans;
		trans.mPosition			= vec2(16.0f);
		trans.mRotation			= 0.0f;
		trans.mRotationPivot	= vec2(0.0f);
		trans.mScale			= vec2(11.0f, 16.0f) * 2.0f;

		gfx->DrawText(shader, layer, vp2d, trans, textAtlas, mMsg);

		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	//----------------------------------------------------Text example
	if (mTexture != nullptr)
		delete mTexture;
	//----------------------------------------------------End

	if (mRenderer != nullptr)
	{
		mRenderer->SetWindow(nullptr);
		delete mRenderer;
		mRenderer = nullptr;
	}

	mTime->SetSpeed(nullptr);

	SuperUnload();
}