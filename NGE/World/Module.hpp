/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MODULE_HPP
#define MODULE_HPP
#include <Types.hpp>
#include <String.hpp>
#include "Entity.hpp"

class eDoer;

//! A module that can be used by a doer
//! \ingroup World
class eModule : public eEntity
{
public:
	void	SetParent(eDoer* parent);
	eDoer*	Parent();

private:
	eDoer* mParent;
};
#endif