/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef SCENE_HPP
#define SCENE_HPP
#include <Types.hpp>
#include <String.hpp>
#include "Actor.hpp"
#include <vector>

class eScene
{
public:
							~eScene();

	bool					Initialize();
	void					Update();
	void					Render();
	void					Unload(const bool killPersistent = true);

	void					Load(eString fileName);
	void					Save(eString fileName);

	void					SpawnActor(eString fileName, eString name);
	void					DestroyActor(eString name, bool forceNow = false);
	eActor*					GetActor(eString name);
	std::vector<eActor*>*	GetActors();
	void					SwitchScene(eString fileName);
	void					CancelSwitch();
	void					ApplySwitch();

	void					SetName(eString name);

	void					SetGlobalQuality(eQuality quality);
	eQuality				GetGlobalQuality();

	void					Pause();
	void					Resume();
	bool					IsPaused();

private:
	std::vector<eActor*>	mActors;
	bool					mShouldSwitchScene = false;
	eString					mSwitchSceneTo;
	eString					mName;
	eQuality				mGlobalQuality;
	bool					mPaused;
};
#endif