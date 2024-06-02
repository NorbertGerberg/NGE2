/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef ALARM_HPP
#define ALARM_HPP
#include <Types.hpp>
#include <String.hpp>
#include <functional>
#include <unordered_map>

#define ALARM_BIND(fnc) std::bind(&fnc, this)

struct eAlarmModule
{
	std::function<void()>	mFnc;
	int32					mCnt = 0;
};

class eAlarm
{
public:
	void Push(eString name, std::function<void()> fnc);
	void Update();
	void Set(eString name, int32 frames);

private:
	std::unordered_map<strg, eAlarmModule> mAlarms;
};
#endif