/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef VIEWPORT3D_HPP
#define VIEWPORT3D_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
#include <bgfx/bgfx.h>
#include "Transformations.hpp"

//! Ortho view info
//! \ingroup Engine
struct eOrthoView
{
	real3 mLeft		= -15.0f;
	real3 mRight	= 15.0f;
	real3 mBottom	= -15.0f;
	real3 mTop		= 15.0f;
};

//! Viewport class for 3D
//! \ingroup Engine
class eViewport3D : public eBase
{
public:
	eViewport3D();
	void Update(bgfx::ViewId viewId, real3 aspect, real3 zNear, real3 zFar);
	void UpdateMV(bgfx::ViewId viewId, real3 aspect, real3 zNear, real3 zFar);
	void UpdateOrtho(bgfx::ViewId viewId, eOrthoView ov, real3 zNear, real3 zFar);

	void MouseRotate(real3 xpos, real3 ypos);

	void SetTranslation(vec3 translation);
	vec3 GetTranslation();

	void SetFront(vec3 front);
	vec3 GetFront();

	void SetTarget(vec3 target);
	vec3 GetTarget();

	void SetUp(vec3 up);
	vec3 GetUp();

	void SetDirection(eDirection direction);
	eDirection GetDirection();

	void SetZoom(real3 zoom);
	real3 GetZoom();

	void SetMinMaxZoom(vec2 minMaxZoom);
	vec2 GetMinMaxZoom();

	void SetMinMaxPitch(vec2 minMaxPitch);
	vec2 GetMinMaxPitch();

	void SetMinMaxYaw(vec2 minMaxYaw);
	vec2 GetMinMaxYaw();

	void SetSensitivity(real3 sensitivity);
	real3 GetSensitivity();

	void SetView(mat4 view);
	mat4 GetView();

	void SetFirstMouse(bool vl = true);

	void SetOrbitMode(bool vl);
	bool GetOrbitMode();

	void SetOrbitRadius(real3 radius);
	real3 GetOrbitRadius();

private:
	vec3 mTranslation;
	vec3 mFront;
	vec3 mTarget;
	vec3 mUp;
	eDirection mDirection;
	vec2 mMinMaxPitch;
	vec2 mMinMaxYaw;
	real3 mSensitivity;
	real3 mZoom;
	vec2 mMinMaxZoom;

	mat4 mView;

	real3 mLastX;
	real3 mLastY;
	bool  bFirstMouse;

	bool	bOrbitRot;
	real3	mOrbitRadius;
};
#endif