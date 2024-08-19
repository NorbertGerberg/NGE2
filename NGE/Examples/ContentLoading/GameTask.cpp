/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/GameRenderer.hpp"
#include "GameTask.hpp"

//----------------------------------------------------Content example

//	Function for loading a texture
void* load_texture(eString infoFile, eString name, EDR::Archive* archive)
{
	strg pngImage = name.GetStrg() + ".src";
	strg tmpImage = name.GetStrg() + ".tmp";
	EDR::CopyArchiveFileContents(archive, pngImage.c_str(), tmpImage.c_str(), true);

	eTexture* texture = new eTexture();
	texture->Load(tmpImage, false, 20);

	if(EDR::FileExists(tmpImage.c_str()))
		EDR::RemoveFile(tmpImage.c_str());

	return texture;
}

//	Function for destroying a texture
void destroy_texture(void* ptr)
{
	if(ptr != nullptr)
		delete reinterpret_cast<eTexture*>(ptr);
}

//----------------------------------------------------End

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 1280;
	uint32 height = 720;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("Content Loading");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	bQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);


	//----------------------------------------------------Content example

	//	Init texture asset type
	eAssetFncs texFnc;
	texFnc.mConstr		= load_texture;
	texFnc.mDeconstr	= destroy_texture;
	ePackage::AddFnc("texture", texFnc);

	//	Init content
	mContent = new eContent();
	mContent->Initialize("pck");

	// Load package
	if (EDR::FileExists("MyPack.pck"))
		mContent->LoadPck("", "MyPack");
	else
	{
		// Create Package and insert assets
		mContent->CreatePck("", "MyPack");
		mContent->GetPck("MyPack")->InsertAsset("test_texture.png", "test_texture", "", "texture");
		mContent->GetPck("MyPack")->InsertAsset("test_cursor.png", "test_cursor", "", "texture");
	}

	//	Multithreaded package loading:
	//
	//  std::vector<ePackageInfo> packages;
	//  packages.push_back({ "", "MyPack" });
	//  packages.push_back({ "", "OtherPack" });
	//  mContent->Load(packages);

	//----------------------------------------------------End

	return true;
}

void GameTask::Update()
{
	mTime->Update();
	bQuit = mWnd->WindowCloseRequested();

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
			
		//----------------------------------------------------Content example
			eDI::Begin("Hello", nullptr, ImGuiWindowFlags_NoSavedSettings);

				eTexture* tex = asset_cast(eTexture, mContent->GetPck("MyPack")->GetAsset("test_texture"));

				eDI::Image((ImTextureID)tex->GetHandle().idx, ImVec2(128, 128));
				tex = nullptr;

				tex = asset_cast(eTexture, mContent->GetPck("MyPack")->GetAsset("test_cursor"));

				eDI::Image((ImTextureID)tex->GetHandle().idx, ImVec2(44, 76));
				tex = nullptr;

			eDI::End();
		//----------------------------------------------------End

		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	//----------------------------------------------------Content example
	if (mContent != nullptr)
	{
		delete mContent;
		mContent = nullptr;
	}
	ePackage::ClearAllFnc();
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