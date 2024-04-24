/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Content.hpp"
#include <System.hpp>

eContent::~eContent()
{
	ReleaseAll();
}

bool eContent::LoadPck(eString path, eString name)
{
	for (auto& it : mPackages)
	{
		if (it.first.Compare(name))
			return false;
	}

	ePackage* pck = new ePackage();
	if (!pck->Load(path, name))
		return false;
	mPackages.push_back(std::make_pair(name, pck));
	return true;
}

void eContent::LoadPckMT(eString path, eString name)
{
	for (auto& it : mPackages)
	{
		if (it.first.Compare(name))
			return;
	}

	ePackage* pck = new ePackage();
	pck->Load(path, name);

	std::lock_guard<std::mutex> lock(mMutex);

	mPackages.push_back(std::make_pair(name, pck));
}

bool eContent::Load(std::vector<std::pair<eString, eString>>& packages)
{
	const uint cpus = eSystem::GetProcessorCount();

	std::vector<std::thread> threads;

	std::vector<std::pair<eString, eString>> others;
	uint threadCnt = 0;
	for (const auto& it : packages)
	{
		if (threadCnt > cpus)
		{
			others.push_back(it);
			continue;
		}
		threads.push_back(std::thread(&eContent::LoadPckMT, this, it.first, it.second));
		threadCnt++;
	}

	for (auto& it : threads)
		it.join();

	if (!others.empty())
		Load(others);
	return true;
}

bool eContent::LoadST(std::vector<std::pair<eString, eString>>& packages)
{
	for (auto& it : packages)
	{
		const bool rtVl = LoadPck(it.first, it.second);
		if (!rtVl)
			return false;
	}
	return true;
}

void eContent::ReleaseAll()
{
	for (auto& it : mPackages)
	{
		if (it.second != nullptr)
			delete it.second;
		it.first.Clear();
	}
	mPackages.clear();
}

void eContent::ReleasePck(eString name)
{
	for (uint i = 0; i < mPackages.size(); i++)
	{
		auto& it = mPackages[i];
		if (it.first.Compare(name))
		{
			if (it.second != nullptr)
				delete it.second;
			it.first.Clear();
			mPackages.erase(mPackages.begin() + i);
			return;
		}
	}
}

uint64 eContent::GetSize()
{
	return mPackages.size();
}

std::vector<eString> eContent::GetPckNames()
{
	std::vector<eString> rtVl;
	for (auto& it : mPackages)
		rtVl.push_back(it.first);
	return rtVl;
}

std::vector<std::pair<eString, ePackage*>>* eContent::GetPackages()
{
	return &mPackages;
}

ePackage* eContent::GetPackage(eString name)
{
	for (auto& it : mPackages)
	{
		if (name.Compare(it.first))
			return it.second;
	}
	return nullptr;
}

void eContent::CreatePackage(eString path, eString name)
{
	strg fileName = path.GetStrg() + name.GetStrg() + ".pck";
	if (EDR::FileExists(fileName.c_str()))
		EDR::RemoveFile(fileName.c_str());

	EDR::Archive* archive = EDR::CreateArchive(fileName.c_str());
	strg infoFile = "<" + name.GetStrg() + ">\n</" + name.GetStrg() + ">";
	EDR::AddFileToArchive(archive, "info.rd", infoFile.c_str());

	archive = EDR::CloseArchive(archive);
}