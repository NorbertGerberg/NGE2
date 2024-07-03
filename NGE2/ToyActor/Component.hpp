/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Console.hpp>
#include "Property.hpp"
#include "Object.hpp"
#include "Actor.hpp"
#include "Alarm.hpp"
#include <vector>

#define component_cast(T, V)	dynamic_cast<T*>(V)

#define expands : public
#define defaultproperties(I) I::I()
#define defaultinit(I) I(); \
						bool Initialize(); \
						void Update();     \
						void Render();     \
						void Unload();     \

struct eCmpProperty
{
	eProperty	mProperty;
	eString		mAssetType;
};

class eComponent : public eObject
{
public:
	virtual						~eComponent() = default;
	virtual						bool Initialize() = 0;
	virtual						void Update() = 0;
	virtual						void Render() = 0;
	virtual						void Unload() = 0;

	void						SuperInit();
	void						SuperUnload();

	void						SetNickName(eString nickName);
	eString						GetNickName();

	void						UseUpdate(bool vl);
	void						UseRender(bool vl);
	bool						CanUpdate();
	bool						CanRender();

	void						SetParent(eActor* parent);
	eActor*						GetParent();

	static void					SetGame(void* game);
	static void*				GetGame();

	template<typename T>
	void InitProperty(eString name, const T& value, ePropertyType type, eString assetType = "default")
	{
		for (auto& it : mProperties)
		{
			if (it.mProperty.mName.Compare(name))
			{
				eConsole::PrintLog("WARNING::eComponent::InitProperty", "Property '" + name.GetStrg() + "' already exists inside '" + GetName().GetStrg() + "'!");
				return;
			}
		}

		eProperty rtPrp;
		rtPrp.mName = name;
		rtPrp.mType = type;
		rtPrp.mValue = new T(value);

		eCmpProperty rtVl;
		rtVl.mProperty = rtPrp;
		rtVl.mAssetType = assetType;

		mProperties.push_back(rtVl);
	}

	template<typename T>
	T GetProperty(eString name)
	{
		for (auto& it : mProperties)
		{
			if (it.mProperty.mName.Compare(name))
				return *reinterpret_cast<T*>(it.mProperty.mValue);
		}
		eConsole::PrintLog("WARNING::eComponent::GetProperty", "Property '" + name.GetStrg() + "' could not be found inside '" + GetName().GetStrg() + "'!");
		return T();
	}

	template<typename T>
	void SetProperty(eString name, const T& value)
	{
		for (auto& it : mProperties)
		{
			if (it.mProperty.mName.Compare(name))
			{
				auto& prp = it.mProperty.mValue;
				if (it.mProperty.mType == ePropertyType::tString)
					delete reinterpret_cast<eString*>(prp);
				else
					delete prp;
				prp = new T(value);
				return;
			}
		}
		eConsole::PrintLog("WARNING::eComponent::SetProperty", "Property '" + name.GetStrg() + "' could not be found inside '" + GetName().GetStrg() + "'!");
	}

	std::vector<eCmpProperty>*	GetProperties();
	eCmpProperty*				GetPropertyBase(eString name);

	eAlarm*						mAlarm;

protected:
	eString						mNickName;
	bool						mUseUpdate;
	bool						mUseRender;
	eActor*						mParent;
	bool						mAwake;
	static void*				mGame;

	std::vector<eCmpProperty>	mProperties;

	bool						mDestroyRequested;
};
#endif