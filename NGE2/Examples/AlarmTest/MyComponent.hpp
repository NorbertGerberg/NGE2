/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MYCOMPONENT_HPP
#define MYCOMPONENT_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Component.hpp>

class MyComponent expands eComponent
{
public:
	defaultinit(MyComponent)

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