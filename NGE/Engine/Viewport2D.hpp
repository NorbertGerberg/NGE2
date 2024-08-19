/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef VIEWPORT2D_HPP
#define VIEWPORT2D_HPP
#include <Types.hpp>
#include <Base.hpp>

//! Viewport class for 2D
//! \ingroup Engine
class eViewport2D : public eBase
{
public:
	void SetTranslation(vec2 translation);
	vec2 GetTranslation();

	void SetSize(vec2 size);
	vec2 GetSize();

private:
	vec2 mTranslation;
	vec2 mSize;
};
#endif