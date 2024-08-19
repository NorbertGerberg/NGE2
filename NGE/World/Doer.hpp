/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef DOER_HPP
#define DOER_HPP
#include <Types.hpp>
#include <String.hpp>
#include "UniqueMap.hpp"
#include "Entity.hpp"
#include "Properties.hpp"
#include "Tags.hpp"
#include "Module.hpp"
#include <memory>

class eWorld;

//! An entity holding modules
//! \ingroup World
class eDoer : public eEntity
{
public:
	eTags& Tags();

	template<typename T>
	void AddModule(eString name)
	{
		if (mModules.Find(name)) return;
		mModules.Push(std::make_unique<T>(), name);
	}

	void RemoveModule(eString name);
	eModule* GetModule(eString name);
	eUniqueMap<eModule>& GetModules();

	void	SetWorld(eWorld* world);
	eWorld* World();

	void	Destroy(const bool vl = true);
	bool	ShouldDestroy();

	eString				mClassName;

private:
	eTags				mTags;
	eUniqueMap<eModule>	mModules;
	eWorld*				mWorld;
	bool				bShouldDestroy = false;
};
#endif