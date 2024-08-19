/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <Types.hpp>
/*#include <bgfx/bgfx.h>
#include <bx/math.h>
#include "Window.hpp"*/
#include <bgfx/bgfx.h>
#include "Transformations.hpp"

//! Usefull functions
//! \ingroup Engine
namespace eTools
{
	template<typename T>
	T mini(T x, T y)
	{
		return x < y ? x : y;
	}

	template<typename T>
	T maxi(T x, T y)
	{
		return x > y ? x : y;
	}

	const real3 CalcDistance(vec3 d1, vec3 d2);
	mat4 Transform(eTransformation3D transformation);
	eTransformation3D Decompose(mat4 mat);

	mat4 GetWorldTransform(eTransformation3D& parent, eTransformation3D& child);
	eTransformation3D GetWorldTransformAlt(eTransformation3D& parent, eTransformation3D& child);
	
	bgfx::TextureHandle Framebuffer2Texture(bgfx::TextureHandle fb, vec2i size, bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8, uint64 flags = 0);

	vec3 Get3DRaycastedMousePosition(vec2 mousePos, const real3 screenWidth, const real3 screenHeight,
		vec3 cameraPos, vec3 cameraUp, vec3 cameraFront, real3 cameraZoomVl, const real3 distance);
	vec3 Get3DRaycastedPosition(const real3 X, const real3 Y, const real3 screenWidth, const real3 screenHeight,
		vec3 cameraPos, vec3 cameraUp, vec3 cameraFront, real3 cameraZoomVl, const real3 distance);
	vec2 Get2DCoordFrom3DSpace(vec3 pos, const real3 screenWidth, const real3 screenHeight, vec3 cameraPos,
		vec3 cameraFront, vec3 cameraUp, real3 cameraZoomVl, const bool followScreen, vec2 size = vec2(0.0f));

	const bool Compare(eTransformation3D first, eTransformation3D last);
	const bool Compare(vec3 first, vec3 last);

	const vec3 ColorToVec3(const eColor& color);
	const vec4 Color4ToVec4(const eColor4& color);
	const eColor Vec3ToColor(const vec3& vl);
	const eColor4 Vec4ToColor4(const vec4& vl);
};
#endif