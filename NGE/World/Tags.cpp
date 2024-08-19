/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Tags.hpp"
#include <Console.hpp>

void eTags::Push(eString tag)
{	 
	if (Has(tag))
		eConsole::PrintLog("INFO::eTags::Push", "Tag already exists!");
	else mItems.push_back(tag);
}	 

void eTags::Pop(eString tag)
{	 
	for (uint i = 0; i < mItems.size(); i++)
	{
		if (mItems[i] == tag)
		{
			mItems.erase(mItems.begin() + i);
			return;
		}
	}
	eConsole::PrintLog("WARNING::eTags::Pop", "Tag does not exists!");
}	

void eTags::Popa()
{
	mItems.clear();
}

bool eTags::Has(eString tag)
{
	for (auto& it : mItems)
	{
		if (it == tag)
			return true;
	}
	return false;
}

std::vector<eString>& eTags::GetCollection()
{
	return mItems;
}