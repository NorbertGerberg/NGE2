/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Module.hpp"
#include "Doer.hpp"

void eModule::SetParent(eDoer* parent)
{
	mParent = parent;
}

eDoer* eModule::Parent()
{
	return mParent;
}