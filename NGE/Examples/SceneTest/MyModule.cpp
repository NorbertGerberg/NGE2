/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "MyModule.hpp"
#include <Graphics.hpp>

SUPERENTITY(eModule)
MyModule::MyModule()
{
	mMsg = "Hello, World!";
	Properties().Init("Msg", &mMsg);
}

bool MyModule::Initialize()
{
	Super::Initialize();
	bCanRender = true;
	return true;
}

void MyModule::Render()
{
	eDI::Begin("Hello", nullptr, ImGuiWindowFlags_NoSavedSettings);
	eDI::Text(mMsg.Get());
	eDI::End();
}