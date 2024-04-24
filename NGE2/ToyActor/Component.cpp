/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Component.hpp"

void* eComponent::mGame = nullptr;

void eComponent::SuperInit()
{
	mUseUpdate			= false;
	mUseRender			= false;
	mDestroyRequested	= false;
	Sleep();
}

void eComponent::SuperUnload()
{
	for (auto& it : mProperties)
	{
		if (it.mProperty.mValue != nullptr)
		{
			void* ptr = it.mProperty.mValue;
			if (it.mProperty.mType == ePropertyType::tString)
				delete reinterpret_cast<eString*>(ptr);
			else
				delete ptr;
			ptr = it.mProperty.mValue = nullptr;
		}
	}
	mProperties.clear();
}

void eComponent::SetNickName(eString nickName)
{
	mNickName.Clear();
	mNickName = nickName;
}

eString eComponent::GetNickName()
{
	return mNickName;
}

void eComponent::UseUpdate(bool vl)
{
	mUseUpdate = vl;
}

void eComponent::UseRender(bool vl)
{
	mUseRender = vl;
}

bool eComponent::CanUpdate()
{
	return mUseUpdate;
}

bool eComponent::CanRender()
{
	return mUseRender;
}

void eComponent::SetParent(eActor* parent)
{
	mParent = nullptr;
	mParent = parent;
}

eActor* eComponent::GetParent()
{
	return mParent;
}

std::vector<eCmpProperty>* eComponent::GetProperties()
{
	return &mProperties;
}

void eComponent::SetGame(void* game)
{
	mGame = nullptr;
	mGame = game;
}

void* eComponent::GetGame()
{
	return mGame;
}

eCmpProperty* eComponent::GetPropertyBase(eString name)
{
	for (auto& it : mProperties)
	{
		if (it.mProperty.mName.Compare(name))
			return &it;
	}
	return nullptr;
}