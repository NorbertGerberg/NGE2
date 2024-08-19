/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Viewport2D.hpp"

void eViewport2D::SetTranslation(vec2 translation)
{
	mTranslation = translation;
}

vec2 eViewport2D::GetTranslation()
{
	return mTranslation;
}

void eViewport2D::SetSize(vec2 size)
{
	mSize = size;
}

vec2 eViewport2D::GetSize()
{
	return mSize;
}