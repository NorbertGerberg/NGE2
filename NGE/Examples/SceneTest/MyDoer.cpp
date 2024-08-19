/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "MyDoer.hpp"
#include "MyModule.hpp"
#include <World.hpp>

SUPERENTITY(eDoer)
MyDoer::MyDoer()
{
	AddModule<MyModule>("MyMod");
}

bool MyDoer::Initialize()
{
	Super::Initialize();
	GetModule("MyMod")->Properties().Set("Msg", eString("I modified this!"));

	bCanUpdate = true;					// Must be true for Alarm System to work!
	Alarm().Push("Destroy Event", ALARM_BIND(MyDoer::DestroyEvent));
	Alarm().Set("Destroy Event", 180);	//	3 seconds on 60FPS
	return true;
}

void MyDoer::DestroyEvent()
{
	Destroy();

	if (World()->GetDoer("Another Doer") != nullptr) return;
	MyDoer* newDoer = World()->SpawnDoer<MyDoer>("Another Doer");
	newDoer->Alarm().Set("Destroy Event", -1);
	newDoer->GetModule("MyMod")->Properties().Set("Msg", eString("I am different!"));
}