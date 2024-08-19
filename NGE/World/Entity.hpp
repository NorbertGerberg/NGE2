/*=========================================
	Copyright 2024-2025 Norbert Gerberg

	Base class for Doer and Module
=========================================*/
#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <Types.hpp>
#include <String.hpp>
#include "Object.hpp"
#include "Properties.hpp"
#include "Alarm.hpp"

#define SUPERENTITY(I) using Super = I;

//! Base class for doers and modules
//! \ingroup World
class eEntity : public eObject
{
public:
	eProperties&	Properties();
	eAlarm&			Alarm();

	bool		bCanUpdate = false;
	bool		bCanRender = false;

private:
	eProperties mProperties;
	eAlarm		mAlarm;
};
#endif 