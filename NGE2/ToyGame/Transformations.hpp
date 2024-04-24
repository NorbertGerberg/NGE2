/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef TRANSFORMATIONS_HPP
#define TRANSFORMATIONS_HPP
#include <Types.hpp>

struct eDirection
{
	real3 mYaw;
	real3 mPitch;
	real3 mRoll;
};

struct eTransformation2D
{
	vec2	mPosition;
	vec2	mScale;
	real3	mRotation;
	vec2	mRotationPivot;
};

struct eTransformation3D
{
	vec3 mPosition;						//1 unit = 1 centimeter
	vec3 mRotation;						//In degrees
	vec3 mScale = vec3(1.0f);			//1 unit = 1 centimeter
};
#endif