/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef WORLD_HPP
#define WORLD_HPP
#include <Types.hpp>
#include <String.hpp>
#include <EDR.hpp>
#include "UniqueMap.hpp"
#include "Object.hpp"
#include "Doer.hpp"
#include "Factory.hpp"
#include <memory>
#include <functional>

//! Property type data for saving and loading
//! \ingroup World
struct ePropertyType
{
	std::function<eString(void*)>		mOnSave;
	std::function<void(void*,eString)>	mOnLoad;
};

//! World class for managing doers
//! \ingroup World
class eWorld : public eObject
{
public:
	bool Initialize();
	void Update();
	void Render();
	void Unload();

	template<typename T>
	bool InsertDoer(eString name)
	{
		if (mDoers.Find(name)) return false;
		mDoers.Push(std::make_unique<T>(), name);
		mDoers[name]->mClassName = typeid(T).name();
		return true;
	}

	template<typename T>
	T* SpawnDoer(eString name)
	{
		if (!InsertDoer<T>(name)) return nullptr;
		eDoer* it = mDoers[name];
		for (auto& it2 : it->GetModules().Geta())
		{
			auto& it3 = it2.second;
			if (!it3->Initialize())
			{
				eConsole::PrintLog("ERROR::eWorld::SpawnDoer", "Module '" + it2.first.GetStrg() + "' inside '" + name.GetStrg() + "' returned false!");
				eConsole::WriteToDisk();
			}
			it3->Awake();
		}
		if (!it->Initialize())
		{
			eConsole::PrintLog("ERROR::eWorld::SpawnDoer", "Doer '" + name.GetStrg() + "' returned false!");
			eConsole::WriteToDisk();
		}
		it->Awake();
		return reinterpret_cast<T*>(it);
	}

	eDoer* GetDoer(eString name);
	static eFactory<eDoer>& Factory();

	void Save(eString outfile);
	void Load(eString filepath, const bool forceload = false);

	virtual eString OnSave_info();
	virtual void OnSave(EDR::Archive* archive);
	virtual void OnLoad_info(eString& info);
	virtual void OnLoad(EDR::Archive* archive);

	template<typename T>
	static void RegisterPropertyType(ePropertyType type)
	{
		strg name(typeid(T).name());
		if (mPropTypes.Find(name)) return;
		mPropTypes.Push(type, name);
	}
	static void PopPropertyTypes();

	static void INIT();

	eString GetFilepath();

private:
	eUniqueMap<eDoer>			mDoers;
	eString						mFilepath;
	
	static eFactory<eDoer>		mFactory;
	static eMap<ePropertyType>	mPropTypes;
};
#endif