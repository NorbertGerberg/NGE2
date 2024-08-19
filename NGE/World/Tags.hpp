/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef TAGS_HPP
#define TAGS_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
#include <vector>

//! Class for managing doer tags
//! \ingroup World
class eTags : public eBase
{
public:
	void					Push(eString tag);
	void					Pop(eString tag);
	void					Popa();
	bool					Has(eString tag);
	std::vector<eString>&	GetCollection();

private:
	std::vector<eString> mItems;
};
#endif