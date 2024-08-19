/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef UNIQUEMAP_HPP
#define UNIQUEMAP_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Console.hpp>
#include <Base.hpp>
#include <vector>
#include <memory>

//! Map class for holding doers
//! \ingroup World
template<typename T>
class eUniqueMap : public eBase
{
public:
	void Push(std::unique_ptr<T> vl, eString id = "")
	{
		eString newId = std::to_string(mItems.size() + 1);
		if (!id.IsEmpty())
		{
			newId = id;
			for (auto& it : mItems)
			{
				if (it.first == newId)
				{
					eConsole::PrintLog("WARNING::eUniqueMap::Push", "Item '" + newId.GetStrg() + "' already exists!");
					return;
				}
			}
		}

		mItems.push_back(std::make_pair(newId, std::move(vl)));
	}

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
		eConsole::PrintLog("WARNING::eUniqueMap::Pop", "Item '" + id.GetStrg() + "' could not be found!");
	}

	void Pop(uint id)
	{
		if (id > mItems.size() || id < 0)
		{
			eConsole::PrintLog("WARNING::eUniqueMap::Pop", "Invalid id '" + std::to_string(id) + "'!");
			return;
		}
		mItems.erase(mItems.size() + id);
	}

	void Popa()
	{
		mItems.clear();
	}

	T* Get(eString id)
	{
		for (auto& it : mItems)
		{
			if (it.first == id)
				return it.second.get();
		}
		eConsole::PrintLog("WARNING::eUniqueMap::Get", "Item '" + id.GetStrg() + "' could not be found!");
		return nullptr;
	}

	T* Get(uint id)
	{
		if (id > mItems.size() || id < 0)
		{
			eConsole::PrintLog("ERROR::eUniqueMap::Get", "Invalid id '" + std::to_string(id) + "'!");
			eConsole::WriteToDisk();
		}
		return mItems[id].second.get();
	}

	std::vector<std::pair<eString, std::unique_ptr<T>>>& Geta()
	{
		return mItems;
	}

	T* operator [](eString id)
	{
		return Get(id);
	}

	T* operator [](uint id)
	{
		return Get(id);
	}

	uint64 Size()
	{
		return mItems.size();
	}

	eString& IdAt(uint i)
	{
		if (i > mItems.size() || i < 0)
		{
			eConsole::PrintLog("ERROR::eUniqueMap::IdAt", "Invalid id '" + std::to_string(i) + "'!");
			eConsole::WriteToDisk();
		}
		return mItems[i].first;
	}

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
	std::vector<std::pair<eString, std::unique_ptr<T>>>	mItems;
};
#endif