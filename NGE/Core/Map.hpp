/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef MAP_HPP
#define MAP_HPP
#include "Types.hpp"
#include "String.hpp"
#include "Base.hpp"
#include "Console.hpp"
#include <vector>

//! Map container for objects
//! \ingroup Core
template<typename T>
class eMap : public eBase
{
public:
	//! Push an item to the collection
	void Push(T vl, eString id = "")
	{
		eString newId = std::to_string(mItems.size() + 1);
		if (!id.IsEmpty())
		{
			newId = id;
			for (auto& it : mItems)
			{
				if (it.first == newId)
				{
					eConsole::PrintLog("WARNING::eMap::Push", "Item '" + newId.GetStrg() + "' already exists!");
					return;
				}
			}
		}

		mItems.push_back(std::make_pair(newId, vl));
	}

	//! Pop an item from the collection
	void Pop(eString id)
	{
		for (uint i = 0; i < mItems.size(); i++)
		{
			auto& it = mItems[i];
			if (it.first == id)
			{
				mItems.erase(mItems.begin() + i);
				return;
			}
		}
		eConsole::PrintLog("WARNING::eMap::Pop", "Item '" + id.GetStrg() + "' could not be found!");
	}

	//! Pop an item from the collection by number index
	void Pop(uint id)
	{
		if (id > mItems.size() || id < 0)
		{
			eConsole::PrintLog("WARNING::eMap::Pop", "Invalid id '" + std::to_string(id) + "'!");
			return;
		}
		mItems.erase(mItems.size() + id);
	}

	//! Clear the collection
	void Popa()
	{
		mItems.clear();
	}

	//! Returns item from collection
	T& Get(eString id)
	{
		for (auto& it : mItems)
		{
			if (it.first == id)
				return it.second;
		}
		eConsole::PrintLog("WARNING::eMap::Get", "Item '" + id.GetStrg() + "' could not be found!");

		T dvl;
		return dvl;
	}

	//! Returns item from collection
	T& Get(uint id)
	{
		if (id > mItems.size() || id < 0)
		{
			eConsole::PrintLog("ERROR::eMap::Get", "Invalid id '" + std::to_string(id) + "'!");
			eConsole::WriteToDisk();
		}
		return mItems[id].second;
	}

	//! Returns collection
	std::vector<std::pair<eString, T>>& Geta()
	{
		return mItems;
	}

	T& operator [](eString id)
	{
		return Get(id);
	}

	T& operator [](uint id)
	{
		return Get(id);
	}

	//! Returns collection size
	uint64 Size()
	{
		return mItems.size();
	}

	//! Returns item name from number index
	eString& IdAt(uint i)
	{
		if (i > mItems.size() || i < 0)
		{
			eConsole::PrintLog("ERROR::eMap::IdAt", "Invalid id '" + std::to_string(i) + "'!");
			eConsole::WriteToDisk();
		}
		return mItems[i].first;
	}

	//! Returns true if item with such a name exists!
	bool Find(eString name)
	{
		for (auto& it : mItems)
		{
			if (it.first == name)
				return true;
		}
		return false;
	}

private:
	std::vector<std::pair<eString, T>>	mItems;
};
#endif