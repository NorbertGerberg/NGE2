/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Viewport3D.hpp"
#include <bx/bx.h>
#include <bx/math.h>

eViewport3D::eViewport3D()
{
	bFirstMouse = true;
	bOrbitRot = false;
	mTranslation = vec3(0.0f);
	mDirection = { 0.0f, 0.0f, 0.0f };
	mUp = vec3(0.0f, 1.0f, 0.0f);
	mZoom = 70.0f;
	mMinMaxZoom = vec2(10.0f, 180.0f);
	mMinMaxPitch = vec2(-89.0f, 89.0f);
	mMinMaxYaw = vec2(-360.0f, 360.0f);
	mSensitivity = 1.0f;
	mOrbitRadius = 400.0f;
	MouseRotate(0.0f, 0.0f);
}

void eViewport3D::Update(bgfx::ViewId viewId, real3 aspect, real3 zNear, real3 zFar)
{
	if (bOrbitRot)
	{
		mView = eMath::translate(mat4(1.0f), vec3(0.0f, 0.0f, mOrbitRadius));
		mView = eMath::rotate(mView, mDirection.mPitch * mSensitivity, vec3(1.0f, 0.0f, 0.0f));
		mView = eMath::rotate(mView, mDirection.mYaw * mSensitivity, vec3(0.0f, 1.0f, 0.0f));
		mView = eMath::translate(mView, -mTranslation);
	}
	else
	{
		mTarget = mTranslation + mFront;
		mView = eMath::lookAtLH(mTranslation, mTarget, mUp);
	}

	float proj[16];
	bx::mtxProj(proj, mZoom, aspect, zNear, zFar, bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(viewId, glm::value_ptr(mView), proj);
}

void eViewport3D::UpdateMV(bgfx::ViewId viewId, real3 aspect, real3 zNear, real3 zFar)
{
	float proj[16];
	bx::mtxProj(proj, mZoom, aspect, zNear, zFar, bgfx::getCaps()->homogeneousDepth);
	bgfx::setViewTransform(viewId, glm::value_ptr(mView), proj);
}

void eViewport3D::UpdateOrtho(bgfx::ViewId viewId, eOrthoView ov, real3 zNear, real3 zFar)
{
	mView = glm::lookAtLH(mTranslation, mTarget, mUp);
	mat4 proj = glm::orthoLH(ov.mLeft, ov.mRight, ov.mBottom, ov.mTop, zNear, zFar);
	bgfx::setViewTransform(viewId, glm::value_ptr(mView), glm::value_ptr(proj));
}

void eViewport3D::MouseRotate(real3 xpos, real3 ypos)
{
	if (bFirstMouse)
	{
		mLastX = xpos;
		mLastY = ypos;
		bFirstMouse = false;
	}

	float xoffset = xpos - mLastX;
	float yoffset = mLastY - ypos;	//Reversed: y ranges bottom to top
	mLastX = xpos;
	mLastY = ypos;

	xoffset *= mSensitivity;
	yoffset *= mSensitivity;

	real3& yaw = mDirection.mYaw;
	real3& pitch = mDirection.mPitch;

	yaw -= xoffset;
	pitch -= yoffset;
	if (pitch >= mMinMaxPitch.y)
		pitch = mMinMaxPitch.y;
	if (pitch <= mMinMaxPitch.x)
		pitch = mMinMaxPitch.x;

	if (yaw >= mMinMaxYaw.y)
		yaw = 0.0f;
	if (yaw <= mMinMaxYaw.x)
		yaw = 0.0f;

	if (bOrbitRot)
	{
		mat4 invView	= eMath::inverse(mView);
		mFront			= eMath::normalize(vec3(invView[2]));
	}
	else
	{
		vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));	//Convert to radians first
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		mFront = glm::normalize(direction);
	}
	mat4 rollMat = glm::rotate(mat4(1.0f), glm::radians(mDirection.mRoll), mFront);
	mUp = mat3(rollMat) * mUp;
}

void eViewport3D::SetTranslation(vec3 translation)
{
	mTranslation = translation;
}

vec3 eViewport3D::GetTranslation()
{
	return mTranslation;
}

void eViewport3D::SetFront(vec3 front)
{
	mFront = front;
}

vec3 eViewport3D::GetFront()
{
	return mFront;
}

void eViewport3D::SetTarget(vec3 target)
{
	mTarget = target;
}

vec3 eViewport3D::GetTarget()
{
	return mTarget;
}

void eViewport3D::SetUp(vec3 up)
{
	mUp = up;
}

vec3 eViewport3D::GetUp()
{
	return mUp;
}

void eViewport3D::SetDirection(eDirection direction)
{
	mDirection = direction;
}

eDirection eViewport3D::GetDirection()
{
	return mDirection;
}

void eViewport3D::SetZoom(real3 zoom)
{
	mZoom = zoom;
}

real3 eViewport3D::GetZoom()
{
	return mZoom;
}

void eViewport3D::SetMinMaxZoom(vec2 minMaxZoom)
{
	mMinMaxZoom = minMaxZoom;
}

vec2 eViewport3D::GetMinMaxZoom()
{
	return mMinMaxZoom;
}

void eViewport3D::SetMinMaxPitch(vec2 minMaxPitch)
{
	mMinMaxPitch = minMaxPitch;
}

vec2 eViewport3D::GetMinMaxPitch()
{
	return mMinMaxPitch;
}

void eViewport3D::SetMinMaxYaw(vec2 minMaxYaw)
{
	mMinMaxYaw = minMaxYaw;
}

vec2 eViewport3D::GetMinMaxYaw()
{
	return mMinMaxYaw;
}

void eViewport3D::SetSensitivity(real3 sensitivity)
{
	mSensitivity = sensitivity;
}

real3 eViewport3D::GetSensitivity()
{
	return mSensitivity;
}

void eViewport3D::SetView(mat4 view)
{
	mView = view;
}

mat4 eViewport3D::GetView()
{
	return mView;
}

void eViewport3D::SetFirstMouse(bool vl)
{
	bFirstMouse = vl;
}

void eViewport3D::SetOrbitMode(bool vl)
{
	bOrbitRot = vl;
}

bool eViewport3D::GetOrbitMode()
{
	return bOrbitRot;
}

void eViewport3D::SetOrbitRadius(real3 radius)
{
	mOrbitRadius = radius;
}

real3 eViewport3D::GetOrbitRadius()
{
	return mOrbitRadius;
}