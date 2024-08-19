/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef FACTORY_HPP
#define FACTORY_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>

//! Factory class for object
//! \ingroup World
template<typename T>
class eFactory : public eBase
{
public:
//!
	template<typename Y>
	void Register()
	{
		mItems[typeid(Y).name()] = []() -> std::unique_ptr<T> {
			return std::make_unique<Y>();
			};
		return;
	}

//!
	std::unique_ptr<T> CreateObject(eString name)
	{
		auto it = mItems.find(name.GetStrg());
		if (it != mItems.end())
			return (it->second)();
		return nullptr;
	}

//!
	bool ClassExists(eString name)
	{
		auto it = mItems.find(name.GetStrg());
		if (it != mItems.end())
			return true;
		return false;
	}

//!
	std::unordered_map<strg, std::function<std::unique_ptr<T>()>>& GetCollection()
	{
		return mItems;
	}

private:
	std::unordered_map<strg, std::function<std::unique_ptr<T>()>> mItems;
};
#endif