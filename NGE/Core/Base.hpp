/*=========================================
	Copyright 2024-2025 Norbert Gerberg

	Base class of all classes
=========================================*/
#ifndef BASE_HPP
#define BASE_HPP
#include "Types.hpp"
#include "Debug.hpp"

//! Base class of all classes
//! \ingroup Core
class eBase
{
public:
	//! Cast class to base
	template<typename T>
	T* Cast(eBase* obj)
	{
		if (obj == nullptr) return nullptr;
		return reinterpret_cast<T*>(obj);
	}
};
#endif