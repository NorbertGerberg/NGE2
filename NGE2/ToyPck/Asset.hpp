/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef ASSET_HPP
#define ASSET_HPP
#include <Types.hpp>
#include <String.hpp>

#define asset_cast(T, V)		reinterpret_cast<T*>(V.mValue)

struct eAsset
{
	eString mName;
	eString mType;
	void*	mValue;
};
#endif