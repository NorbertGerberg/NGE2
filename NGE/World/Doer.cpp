/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Doer.hpp"
#include "Module.hpp"

eTags& eDoer::Tags()
{
	return mTags;
}

void eDoer::RemoveModule(eString name)
{
	mModules.Pop(name);
}

eModule* eDoer::GetModule(eString name)
{
	if (mModules.Find(name))
		return mModules[name];
	return nullptr;
}

eUniqueMap<eModule>& eDoer::GetModules()
{
	return mModules;
}

void eDoer::SetWorld(eWorld* world)
{
	mWorld = world;
}

eWorld* eDoer::World()
{
	return mWorld;
}

void eDoer::Destroy(const bool vl)
{
	bShouldDestroy = vl;
}

bool eDoer::ShouldDestroy()
{
	return bShouldDestroy;
}