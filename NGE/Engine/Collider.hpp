/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef COLLIDER_HPP
#define COLLIDER_HPP
#include <Types.hpp>

//! Simple colliders
//! \ingroup Engine
namespace eCollider
{
	//! Simple AABB collider
	//! \ingroup Engine
	struct eAABB
	{
		vec3 mMin;
		vec3 mMax;
	};

	//! Simple AABB collider for 2D space
	//! \ingroup Engine
	struct eAABB2D
	{
		vec2 mPos;
		vec2 mSize;
	};

	//! Simple Sphere collider
	//! \ingroup Engine
	struct eSphere
	{
		vec3	mPos;
		real3	mRadius;
	};

	//! Simple 2D circle collider
	//! \ingroup Engine
	struct eCircle
	{
		vec2	mPos;
		real3	mRadius;
	};

	//! Simple OBB collider
	//! \ingroup Engine
	struct eOBB
	{
		vec3 mCenter;
		vec3 mAxes[3];	//	Three axes of the OBB (normalized) (radians)
		vec3 mExtents;	//	Half-lengths along each axis
	};
}
#endif