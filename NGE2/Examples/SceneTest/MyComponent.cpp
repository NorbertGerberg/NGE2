/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "MyComponent.hpp"
#include <Graphics.hpp>

defaultproperties(MyComponent)
{
	InitProperty("Message", eString("Hello, ToyGame!"), ePropertyType::tString);
}

bool MyComponent::Initialize()
{
	SuperInit();

	UseUpdate(false);
	UseRender(true);

	mMessage = GetProperty<eString>("Message");

	return true;
}

void MyComponent::Update()
{
	return;
}

void MyComponent::Render()
{
	eDI::Begin("Your Message", nullptr, ImGuiWindowFlags_NoSavedSettings);
		eDI::Text(mMessage.Get());
	eDI::End();
}

void MyComponent::Unload()
{
	SuperUnload();
}