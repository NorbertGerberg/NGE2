/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP
#include <Types.hpp>

//! Direction
//! \ingroup Engine
struct eDirection
{
	real3 mYaw;
	real3 mPitch;
	real3 mRoll;
};

//! 2D space
//! \ingroup Engine
struct eTransformation2D
{
	vec2	mPosition;
	vec2	mScale;
	real3	mRotation;
	vec2	mRotationPivot;
};

//! 3D space
//! \ingroup Engine
struct eTransformation3D
{
	vec3 mPosition;
	vec3 mRotation;				// In degrees
	vec3 mScale = vec3(1.0f);
};

//! Color
//! \ingroup Engine
struct eColor
{
	real3 mRed;
	real3 mGreen;
	real3 mBlue;

	eColor()
	{
		mRed	= 1.0f;
		mGreen	= 1.0f;
		mBlue	= 1.0f;
	}

	eColor(const eColor& vl)
	{
		mRed	= vl.mRed;
		mGreen	= vl.mGreen;
		mBlue	= vl.mBlue;
	}

	eColor(real3 vl)
	{
		mRed	= vl;
		mGreen	= vl;
		mBlue	= vl;
	}

	eColor(real3 red, real3 green, real3 blue)
	{
		mRed	= red;
		mGreen	= green;
		mBlue	= blue;
	}
};

//! Color with alpha
//! \ingroup Engine
struct eColor4 : public eColor
{
	real3 mAlpha;

	eColor4() : eColor()
	{
		mAlpha = 1.0f;
	}

	eColor4(const eColor4& vl)
	{
		mRed	= vl.mRed;
		mGreen	= vl.mGreen;
		mBlue	= vl.mBlue;
		mAlpha	= vl.mAlpha;
	}

	eColor4(const eColor& vl)
	{
		mRed	= vl.mRed;
		mGreen	= vl.mGreen;
		mBlue	= vl.mBlue;
		mAlpha	= 1.0f;
	}

	eColor4(real3 vl) : eColor(vl)
	{
		mAlpha = 1.0f;
	}

	eColor4(real3 red, real3 green, real3 blue) : eColor(red, green, blue)
	{
		mAlpha = 1.0f;
	}

	eColor4(real3 red, real3 green, real3 blue, real3 alpha) : eColor(red, green, blue)
	{
		mAlpha = alpha;
	}
};
#endif