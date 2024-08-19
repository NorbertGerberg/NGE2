/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
#include <Map.hpp>
#include <typeinfo>

#define property_cast(T, V) reinterpret_cast<T*>(V.mValue)

//! Property data
//! \ingroup World
struct eProperty
{
	eString	mType;
	void*	mValue;
};

//! Class for managing entity properties
//! \ingroup World
class eProperties : public eBase
{
public:
	template<typename T>
	void Init(eString name, T* value)
	{
		if (mItems.Find(name)) return;
		eString type(typeid(T).name());
		mItems.Push({ type, value }, name);
	}

	eProperty&			Get(eString name);
	eMap<eProperty>&	Geta();
	void				Pop(eString name);
	void				Popa();

	template<typename T>
	void Set(eString name, const T& value)
	{
		if (!mItems.Find(name)) return;
		auto& it = mItems[name];
		if (!it.mType.Compare(typeid(T).name())) return;
		T* prp = reinterpret_cast<T*>(it.mValue);
		*prp = value;
	}

private:
	eMap<eProperty> mItems;
};
#endif