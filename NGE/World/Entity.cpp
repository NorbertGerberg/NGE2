/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Entity.hpp"

eProperties& eEntity::Properties()
{
	return mProperties;
}

eAlarm& eEntity::Alarm()
{
	return mAlarm;
}