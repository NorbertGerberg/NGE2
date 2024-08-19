/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Material.hpp"

eMaterial::~eMaterial()
{
	delete mProperties;
}

eMaterialPrp* eMaterial::GetProperties()
{
	return mProperties;
}

void eMaterial::SetCurrentPan(vec2 pan)
{
	mCurrentPan = pan;
}

vec2 eMaterial::GetCurrentPan()
{
	return mCurrentPan;
}

//! When using fixed timestep set argument to 1.0
void eMaterial::Update(real6 deltaTime)
{
	mCurrentPan += mProperties->mUVPan * static_cast<real3>(deltaTime);
	if (mCurrentPan.x >= 1.0f)
		mCurrentPan.x = -1.0f;
	else if (mCurrentPan.x <= -1.0f)
		mCurrentPan.x = 1.0f;

	if (mCurrentPan.y >= 1.0f)
		mCurrentPan.y = -1.0f;
	else if (mCurrentPan.y <= -1.0f)
		mCurrentPan.y = 1.0f;
}

void eMaterial::SetProperties(eMaterialPrp* properties)
{
	mProperties = nullptr;
	mProperties	= properties;
}