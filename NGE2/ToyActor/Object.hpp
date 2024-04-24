/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <Types.hpp>
#include <String.hpp>

class eObject
{
public:
	void	SetName(eString name);
	eString GetName();

	void	Sleep();
	void	Awake();
	bool	IsAwake();

protected:
	eString mName;
	bool	mAwake;
};
#endif