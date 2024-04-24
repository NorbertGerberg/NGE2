/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef ACTOR_HPP
#define ACTOR_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Quality.hpp>
#include "Object.hpp"
#include <vector>
#include <memory>

class eComponent;

class eActor : public eObject
{
public:
	bool										Initialize();
	void										Update();
	void										Render();
	void										Unload();

	void										PushComponent(eString name, eString nickName);
	void										PopComponent(eString nickName);
	std::vector<std::unique_ptr<eComponent>>*	GetComponents();
	eComponent*									GetComponent(eString nickName);
	eComponent*									GetComponentByClass(eString className);

	void										SetClass(eString name);
	eString										GetClass();

	void										SavePrefab(eString name);

	void										SetDetailLvl(eQuality lvl);
	eQuality									GetDetailLvl();

	void										Destroy();
	bool										ShouldDestroy();

	void										SetPersistent(bool vl = true);
	bool										IsPersistent();

private:
	std::vector<std::unique_ptr<eComponent>>	mComponents;

	eString										mClassName;
	eQuality									mDetailLvl;
	bool										mShouldDestroy;
	bool										mPersistent;
};
#endif