/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MATERIALPRP_HPP
#define MATERIALPRP_HPP
#include <Types.hpp>
#include <Texture.hpp>
#include <vector>

struct eMaterialPrp
{
	bool					mTwoSides	= false;		//	Control Backface-culling

	vec2					mUVPan		= vec2(0.0f);
	vec2					mUVScale	= vec2(1.0f);
	vec2					mUVOffset	= vec2(0.0f);
};
#endif