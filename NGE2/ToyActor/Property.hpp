/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef PROPERTY_HPP
#define PROPERTY_HPP
#include <Types.hpp>
#include <String.hpp>

enum ePropertyType
{
	tString,
	tInt,
	tUInt,
	tInt8,
	tUInt8,
	tInt16,
	tUInt16,
	tInt32,
	tUInt32,
	tInt64,
	tUInt64,
	tReal3,
	tReal6,
	tVec2,
	tVec3,
	tVec4,
	tBool
};

struct eProperty
{
	eString			mName;
	ePropertyType	mType;
	void*			mValue;
};
#endif