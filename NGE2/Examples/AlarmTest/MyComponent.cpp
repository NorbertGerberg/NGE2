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

	UseUpdate(true);
	UseRender(true);

	mMessage = GetProperty<eString>("Message");

	//----------------------------------------------------Alarm Test
	mShowMsg = false;

	mAlarm->Push("alarm0", ALARM_BIND(MyComponent::ShowMsgAlarm));
	mAlarm->Set("alarm0", 180);		//	3 seconds on 60FPS
	//----------------------------------------------------End

	return true;
}

void MyComponent::Update()
{
	return;
}

void MyComponent::Render()
{
	//----------------------------------------------------Alarm Test
	if (!mShowMsg) return;
	//----------------------------------------------------End

	eDI::Begin("Your Message", nullptr, ImGuiWindowFlags_NoSavedSettings);
		eDI::Text(mMessage.Get());
	eDI::End();
}

void MyComponent::Unload()
{
	SuperUnload();
}

//----------------------------------------------------Alarm Test
void MyComponent::ShowMsgAlarm()
{
	mShowMsg = true;
}
//----------------------------------------------------End