/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "../Shared/GameRenderer.hpp"
#include <Texture.hpp>
#include <Viewport3D.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <Mouse.hpp>
#include "GameTask.hpp"

//----------------------------------------------------3D Test 
//	Custom Material Properties
struct MyMaterialPrp : public eMaterialPrp
{
	eTexture*	mDiffuse	= nullptr;
};

//	Custom Draw Call
eShader* DrawSingle(eMeshContainer& container, eLayer* layer, eTransformation3D transformation, eMaterial* mat)
{
	//	Check Material
	eMaterial* useMat = container.mMat;

	if (mat != nullptr)
		useMat = mat;

	eMaterialPrp* _matPrp = useMat->GetProperties();
	MyMaterialPrp* matPrp = reinterpret_cast<MyMaterialPrp*>(_matPrp);

	eShader* shader = eMesh::GetRenderer()->GetShader("Unlit");

	if (matPrp->mDiffuse != nullptr)
		shader->SetTexture(0, "tex_diffuse", matPrp->mDiffuse);

	return shader;
}
//----------------------------------------------------End

//----------------------------------------------------3D Test
eViewport3D* mViewport;

void scroll_callback(GLFWwindow* window, real6 xoffset, real6 yoffset)
{
	real3 newRadius = mViewport->GetOrbitRadius() - static_cast<real3>(yoffset);
	if (newRadius < 2.0f)
		newRadius = 2.0f;
	else if (newRadius > 10.0f)
		newRadius = 10.0f;
	mViewport->SetOrbitRadius(newRadius);
}
//----------------------------------------------------End

eTexture*	mDiffuse;
eMaterial*	mMat;
eMesh*		mMesh;

bool GameTask::Initialize()
{
	if (!SuperInit())
		return false;

	uint32 width = 1280;
	uint32 height = 720;

	//	Create window
	mWnd->SetSize(vec2i(width, height));
	mWnd->SetTitle("3D Test");
	mWnd->Initialize();
	mWnd->SetWindowVisibility(true);
	mWnd->CenterWindow();

	//	Init renderer
	mRenderer = new GameRenderer(mWnd, width, height, true, 16, eRenderAPI::Direct3D11);

	mQuit = false;
	mGameSpeed = 1.0f;
	mTime->SetSpeed(&mGameSpeed);

	//----------------------------------------------------3D Test
	eMesh::SetRenderer(mRenderer);

	mDiffuse = new eTexture();
	mDiffuse->Load("3d files/diffuse.png", false, 23);

	mMat = new eMaterial();

	MyMaterialPrp* prp = new MyMaterialPrp();
	prp->mDiffuse = mDiffuse;
	mMat->SetProperties(prp);
	prp = nullptr;

	mMesh = new eMesh();
	mMesh->Load("3d files/helmet.fbx");
	mMesh->SetDrawFnc(DrawSingle);

	for (auto& it : mMesh->GetContainers())
		it.mMat = mMat;

	mViewport = new eViewport3D();
	mViewport->SetOrbitMode(true);
	mViewport->SetOrbitRadius(5.0f);
	mViewport->SetTranslation(vec3(0.0f));
	mViewport->SetSensitivity(0.05f);
	mViewport->SetMinMaxPitch(vec2(-30.0f, 30.0f));
	mRenderer->GetLayer("3D")->SetViewport(mViewport);

	mWnd->GetMouse()->CheckRawMouseMotionSupport();
	mWnd->GetMouse()->SetScrollCallback(scroll_callback);

	//----------------------------------------------------End

	return true;
}

//----------------------------------------------------3D Test
void mouse_callback(GLFWwindow* window, real6 xpos, real6 ypos)
{
	mViewport->MouseRotate(static_cast<real3>(xpos), -static_cast<real3>(ypos));
}
//----------------------------------------------------End

void GameTask::Update()
{
	mTime->Update();
	mQuit = mWnd->WindowCloseRequested();

	//----------------------------------------------------3D Test
	mMat->Update(mTime->DeltaTime());

	mWnd->GetMouse()->SetInputMode(NGE_INPUT_CURSOR, mWnd->GetMouse()->GetButtonPressed(NGE_INPUT_MOUSE_BUTTON_RIGHT) ? NGE_INPUT_CURSOR_DISABLED : NGE_INPUT_CURSOR_NORMAL);
	mWnd->GetMouse()->SetPositionCallback(mWnd->GetMouse()->GetButtonPressed(NGE_INPUT_MOUSE_BUTTON_RIGHT) ? mouse_callback : nullptr);

	if (mWnd->GetMouse()->GetButtonPressed(NGE_INPUT_MOUSE_BUTTON_RIGHT))
		mViewport->SetFirstMouse();
	//----------------------------------------------------End
}

void GameTask::Render()
{
	if (!mWnd->IsWindowIconified())
	{
		mRenderer->PreUpdate(true, 16);

		//----------------------------------------------------3D Test

		eTransformation3D transformation;
		transformation.mPosition	= vec3(0.0f);
		transformation.mRotation	= vec3(-90.0f, 0.0f, 0.0f);
		transformation.mScale		= vec3(1.0f);

		mMesh->Draw(mRenderer->GetLayer("3D"), transformation);
		
		//----------------------------------------------------End

		mRenderer->Update();
	}
}

void GameTask::Unload()
{
	//----------------------------------------------------3D Test
	if (mMesh != nullptr)
		delete mMesh;

	if (mMat != nullptr)
		delete mMat;

	if (mDiffuse != nullptr)
		delete mDiffuse;

	if (mViewport != nullptr)
		delete mViewport;
	//----------------------------------------------------End

	if (mRenderer != nullptr)
	{
		//----------------------------------------------------3D Test
		eMesh::SetRenderer(nullptr);
		//----------------------------------------------------End
		mRenderer->SetWindow(nullptr);
		delete mRenderer;
		mRenderer = nullptr;
	}

	mTime->SetSpeed(nullptr);

	SuperUnload();
}