/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MYCOMPONENT_HPP
#define MYCOMPONENT_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Component.hpp>

class MyComponent : public eComponent
{
public:
	MyComponent();
	bool Initialize();
	void Update();
	void Render();
	void Unload();

	//----------------------------------------------------Alarm Test
	void ShowMsgAlarm();
	//----------------------------------------------------End

private:
	eString mMessage;

	//----------------------------------------------------Alarm Test
	bool	mShowMsg;
	//----------------------------------------------------End
};
#endif