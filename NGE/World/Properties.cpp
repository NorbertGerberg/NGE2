/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Properties.hpp"

eProperty& eProperties::Get(eString name)
{
	return mItems[name];
}

eMap<eProperty>& eProperties::Geta()
{
	return mItems;
}

void eProperties::Pop(eString name)
{
	mItems.Pop(name);
}

void eProperties::Popa()
{
	mItems.Popa();
}