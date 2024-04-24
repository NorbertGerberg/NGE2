/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include <Types.hpp>

namespace eCollider
{
	struct eAABB
	{
		vec3 mMin;
		vec3 mMax;
	};

	struct eAABB2D
	{
		vec2 mPos;
		vec2 mSize;
	};

	struct eSphere
	{
		vec3	mPos;
		real3	mRadius;
	};

	struct eCircle
	{
		vec2	mPos;
		real3	mRadius;
	};
}
#endif