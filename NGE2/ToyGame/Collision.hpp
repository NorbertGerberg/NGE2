/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef COLLISION_HPP
#define COLLISION_HPP
#include <Types.hpp>
#include "Collider.hpp"

enum eCollisionSide
{
	eCsNone,
	eCsLeft,
	eCsRight,
	eCsTop,
	eCsBottom,
	eCsFront,
	eCsBack
};

class eCollision
{
public:
	static const bool PointInAABB(const eCollider::eAABB& box, const vec3& point);
	static const bool AABBInAABB(const eCollider::eAABB& box, const eCollider::eAABB& box2);
	static const bool AABB2DInAABB2D(const eCollider::eAABB2D& rect, const eCollider::eAABB2D& rect2);
	static const bool AABB2DInCircle(const eCollider::eAABB2D& rect, const eCollider::eCircle& circle);
	static const bool CircleInCircle(const eCollider::eCircle& circle, const eCollider::eCircle& circle2);
	static const bool PointInSphere(const eCollider::eSphere& sphere, const vec3& point);
	static const bool SphereInSphere(const eCollider::eSphere& sphere, const eCollider::eSphere& sphere2);
	static const bool SphereInAABB(const eCollider::eSphere& sphere, const eCollider::eAABB& box);

	static eCollisionSide GetCollisionSide(const eCollider::eAABB& box, const eCollider::eAABB& box2);
	static eCollisionSide GetCollisionSide2D(const eCollider::eAABB2D& box, const eCollider::eAABB2D& box2);
};
#endif