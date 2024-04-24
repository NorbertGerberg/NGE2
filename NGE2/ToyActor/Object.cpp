/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Object.hpp"

void eObject::SetName(eString name)
{
	mName.Clear();
	mName = name;
}

eString eObject::GetName()
{
	return mName;
}

void eObject::Sleep()
{
	mAwake = false;
}

void eObject::Awake()
{
	mAwake = true;
}

bool eObject::IsAwake()
{
	return mAwake;
}