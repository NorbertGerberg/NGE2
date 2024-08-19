/*=========================================
	Copyright 2024-2025 Norbert Gerberg

	Base class for all objects
=========================================*/
#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>

//! Base for objects in library
//! \ingroup World
class eObject : public eBase
{
public:
	virtual	~eObject();
	virtual bool Initialize();
	virtual void Update();
	virtual void Render();
	virtual void Unload();

	void Sleep();
	void Awake();
	bool IsAwake();

protected:
	bool bAwake = false;
};
#endif