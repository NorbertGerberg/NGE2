/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Object.hpp"

eObject::~eObject()
{
	Unload();
}

bool eObject::Initialize()
{
	return true;
}

void eObject::Update()
{
	return;
}

void eObject::Render()
{
	return;
}

void eObject::Unload()
{
	return;
}

void eObject::Sleep()
{
	bAwake = false;
}

void eObject::Awake()
{
	bAwake = true;
}

bool eObject::IsAwake()
{
	return bAwake;
}