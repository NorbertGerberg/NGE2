/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Alarm.hpp"

void eAlarm::Push(eString name, std::function<void()> fnc)
{
	if (mAlarms.find(name.GetStrg()) != mAlarms.end()) return;
	eAlarmModule rtVl = { fnc, -1 };
	mAlarms[name.GetStrg()] = rtVl;
}

void eAlarm::Update()
{
	for (auto& it : mAlarms)
	{
		auto& cnt = it.second.mCnt;
		if (cnt == -1) continue;
		cnt--;
		if (cnt <= 0)
		{
			cnt = -1;
			it.second.mFnc();
		}
	}
}

void eAlarm::Set(eString name, int32 frames)
{
	if (mAlarms.find(name.GetStrg()) == mAlarms.end()) return;
	mAlarms[name.GetStrg()].mCnt = frames;
}