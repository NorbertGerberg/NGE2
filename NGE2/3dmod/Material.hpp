/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <Types.hpp>
#include "MaterialPrp.hpp"

class eMaterial
{
public:
					~eMaterial();
	void			SetProperties(eMaterialPrp* properties);
	eMaterialPrp*	GetProperties();
	void			SetCurrentPan(vec2 pan);
	vec2			GetCurrentPan();
	void			Update(real6 deltaTime);

private:
	eMaterialPrp*	mProperties;
	vec2			mCurrentPan;
};
#endif