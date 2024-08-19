/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Collision.hpp"
#include "Tools.hpp"

const bool eCollision::PointInAABB(const eCollider::eAABB& box, const vec3& point)
{
	if (point.x > box.mMin.x && point.x < box.mMax.x &&
		point.y > box.mMin.y && point.y < box.mMax.y &&
		point.z > box.mMin.z && point.z < box.mMax.z)
		return true;
	return false;
}

const bool eCollision::AABBInAABB(const eCollider::eAABB& box, const eCollider::eAABB& box2)
{
	if (box.mMax.x > box2.mMin.x && box.mMin.x < box2.mMax.x && box.mMax.y > box2.mMin.y && box.mMin.y < box2.mMax.y &&
		box.mMax.z > box2.mMin.z && box.mMin.z < box2.mMax.z)
		return true;
	return false;
}

const bool eCollision::PointInAABB2D(const eCollider::eAABB2D& box, const vec2& point)
{
	if (point.x > box.mPos.x && point.x < (box.mPos.x + box.mSize.x) &&
		point.y > box.mPos.y && point.y < (box.mPos.y + box.mSize.y))
		return true;
	return false;
}

const bool eCollision::AABB2DInAABB2D(const eCollider::eAABB2D& rect, const eCollider::eAABB2D& rect2)
{
	const bool collisionX = rect.mPos.x + rect.mSize.x >= rect2.mPos.x &&
		rect2.mPos.x + rect2.mSize.x >= rect.mPos.x;

	const bool collisionY = rect.mPos.y + rect.mSize.y >= rect2.mPos.y &&
		rect2.mPos.y + rect2.mSize.y >= rect.mPos.y;

	return collisionX && collisionY;
}

const bool eCollision::AABB2DInCircle(const eCollider::eAABB2D& rect, const eCollider::eCircle& circle)
{
	const bool collisionX = rect.mPos.x + rect.mSize.x >= circle.mPos.x &&
		circle.mPos.x + circle.mRadius >= rect.mPos.x;

	const bool collisionY = rect.mPos.y + rect.mSize.y >= circle.mPos.y &&
		circle.mPos.y + circle.mRadius >= rect.mPos.y;

	return collisionX && collisionY;
}

const bool eCollision::CircleInCircle(const eCollider::eCircle& circle, const eCollider::eCircle& circle2)
{
	const real3 distance = eMath::sqrt(
		(circle.mPos.x - circle2.mPos.x) * (circle.mPos.x - circle2.mPos.x) +
		(circle.mPos.y - circle2.mPos.y) * (circle.mPos.y - circle2.mPos.y)
	);

	return distance < circle.mRadius + circle2.mRadius;
}

const bool eCollision::PointInSphere(const eCollider::eSphere& sphere, const vec3& point)
{
	const real3 distance = eMath::sqrt(
		(point.x - sphere.mPos.x) * (point.x - sphere.mPos.x) +
		(point.y - sphere.mPos.y) * (point.y - sphere.mPos.y) +
		(point.z - sphere.mPos.z) * (point.z - sphere.mPos.z));

	return distance < sphere.mRadius;
}

const bool eCollision::SphereInSphere(const eCollider::eSphere& sphere, const eCollider::eSphere& sphere2)
{
	const real3 distance = eMath::sqrt(
		(sphere.mPos.x - sphere2.mPos.x) * (sphere.mPos.x - sphere2.mPos.x) +
		(sphere.mPos.y - sphere2.mPos.y) * (sphere.mPos.y - sphere2.mPos.y) +
		(sphere.mPos.z - sphere2.mPos.z) * (sphere.mPos.z - sphere2.mPos.z)
	);

	return distance < sphere.mRadius + sphere2.mRadius;
}

const bool eCollision::SphereInAABB(const eCollider::eSphere& sphere, const eCollider::eAABB& box)
{
	const real3 x = eTools::maxi(box.mMin.x, eTools::mini(sphere.mPos.x, box.mMax.x));
	const real3 y = eTools::maxi(box.mMin.y, eTools::mini(sphere.mPos.y, box.mMax.y));
	const real3 z = eTools::maxi(box.mMin.z, eTools::mini(sphere.mPos.z, box.mMax.z));

	const real3 distance = eMath::sqrt(
		(x - sphere.mPos.x) * (x - sphere.mPos.x) +
		(y - sphere.mPos.y) * (y - sphere.mPos.y) +
		(z - sphere.mPos.z) * (z - sphere.mPos.z)
	);

	return distance < sphere.mRadius;
}

eCollisionSide eCollision::GetCollisionSide(const eCollider::eAABB& box, const eCollider::eAABB& box2)
{
	real3 xOv = eTools::mini(box.mMax.x, box2.mMax.x) - eTools::maxi(box.mMin.x, box2.mMin.x);
	real3 yOv = eTools::mini(box.mMax.y, box2.mMax.y) - eTools::maxi(box.mMin.y, box2.mMin.y);
	real3 zOv = eTools::mini(box.mMax.z, box2.mMax.z) - eTools::maxi(box.mMin.z, box2.mMin.z);
	real3 minOv = eTools::mini(eTools::mini(xOv, yOv), zOv);

	const real3 epsilon = 0.0001f;

	if (fabs(minOv - xOv) < epsilon)
		return (box.mMin.x < box2.mMin.x) ? eCollisionSide::CsRight : eCollisionSide::CsLeft;
	else if (fabs(minOv - yOv) < epsilon)
		return (box.mMin.y < box2.mMin.y) ? eCollisionSide::CsTop : eCollisionSide::CsBottom;
	else
		return (box.mMin.z < box2.mMin.z) ? eCollisionSide::CsFront : eCollisionSide::CsBack;
}

eCollisionSide eCollision::GetCollisionSide2D(const eCollider::eAABB2D& box, const eCollider::eAABB2D& box2)
{
	real3 xOv = eTools::mini(box.mPos.x + box.mSize.x, box2.mPos.x + box2.mSize.x) - eTools::maxi(box.mPos.x, box2.mPos.x);
	real3 yOv = eTools::mini(box.mPos.y + box.mSize.y, box2.mPos.y + box2.mSize.y) - eTools::maxi(box.mPos.y, box2.mPos.y);
	real3 minOv = eTools::mini(xOv, yOv);

	const real3 epsilon = 0.0001f;

	if (fabs(minOv - xOv) < epsilon)
		return (box.mPos.x < box2.mPos.x) ? eCollisionSide::CsRight : eCollisionSide::CsLeft;
	else if (fabs(minOv - yOv) < epsilon)
		return (box.mPos.y < box2.mPos.y) ? eCollisionSide::CsTop : eCollisionSide::CsBottom;

	return eCollisionSide::CsNone;
}

const bool eCollision::OBBInOBB(const eCollider::eOBB& obb, const eCollider::eOBB& obb2)
{
	mat3 axes1	= mat3(obb.mAxes[0], obb.mAxes[1], obb.mAxes[2]);
	mat3 axes2	= mat3(obb2.mAxes[0], obb2.mAxes[1], obb2.mAxes[2]);
	mat3 axes	= eMath::transpose(axes1) * axes2;

	vec3 translation = obb2.mCenter - obb.mCenter;

	for (int i = 0; i < 3; i++)
	{
		real3 t = eMath::dot(translation, axes[i]);

		real3 obb1Proj = eMath::dot(glm::abs(obb.mAxes[i]), obb.mExtents);
		real3 obb2Proj = eMath::dot(glm::abs(obb2.mAxes[i]), obb2.mExtents);

		if (eMath::abs(t) > obb1Proj + obb2Proj)
			return false;
	}

	axes = eMath::transpose(axes2) * axes1;

	for (int i = 0; i < 3; i++)
	{
		real3 t = eMath::dot(translation, axes[i]);

		real3 obb1Proj = eMath::dot(glm::abs(obb.mAxes[i]), obb.mExtents);
		real3 obb2Proj = eMath::dot(glm::abs(obb2.mAxes[i]), obb2.mExtents);

		if (eMath::abs(t) > obb1Proj + obb2Proj)
			return false;
	}

	return true;
}

eCollider::eOBB eCollision::AABBToOBB(const eCollider::eAABB& box)
{
	eCollider::eOBB obb;
	obb.mCenter		= (box.mMin + box.mMax) * 0.5f;
	obb.mExtents	= (box.mMax - box.mMin) * 0.5f;
	obb.mAxes[0]	= vec3(1.0f, 0.0f, 0.0f);
	obb.mAxes[1]	= vec3(0.0f, 1.0f, 0.0f);
	obb.mAxes[2]	= vec3(0.0f, 0.0f, 1.0f);
	return obb;
}

const bool eCollision::AABBInOBB(const eCollider::eAABB& box, const eCollider::eOBB& obb)
{
	eCollider::eOBB obb2 = AABBToOBB(box);
	return OBBInOBB(obb, obb2);
}