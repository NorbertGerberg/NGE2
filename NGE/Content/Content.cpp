/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Content.hpp"
#include <Console.hpp>
#include <System.hpp>

eContent::~eContent()
{
	Release();
}

void eContent::Initialize(eString extension)
{
	mExt = extension;
}

void eContent::Release()
{
	for (auto& it : mPackages)
	{
		it->Release();
		delete it;
		it = nullptr;
	}
	mPackages.clear();
}

void eContent::ReleasePck(eString name)
{
	for (uint i = 0; i < mPackages.size(); i++)
	{
		auto& it = mPackages[i];
		if (it->GetName().Compare(name))
		{
			it->Release();
			delete it;
			it = nullptr;
			mPackages.erase(mPackages.begin() + i);
			return;
		}
	}
	eConsole::PrintLog("WARNING::eContent::ReleasePck", "Package '" + name.GetStrg() + "' does not exist!");
}

//! Load single package
void eContent::LoadPck(eString path, eString name)
{
	ePackage* rtVl = new ePackage();
	rtVl->Initialize(path, name, mExt);
	rtVl->Load();
	mPackages.push_back(rtVl);
}

void eContent::CreatePck(eString path, eString name)
{
	ePackage* rtVl = new ePackage();
	rtVl->Initialize(path, name, mExt);
	mPackages.push_back(rtVl);
}

std::vector<ePackage*>& eContent::GetCollection()
{
	return mPackages;
}

ePackage* eContent::GetPck(eString name)
{
	for (auto& it : mPackages)
	{
		if (it->GetName().Compare(name))
			return it;
	}
	eConsole::PrintLog("WARNING::eContent::GetPck", "Package '" + name.GetStrg() + "' could not be found!");
	return nullptr;
}

//! Load packages using multiple CPUs
void eContent::LoadPckMT(ePackageInfo info)
{
	for (auto& it : mPackages)
	{
		if (it->GetName().Compare(info.mName))
			return;
	}

	ePackage* pck = new ePackage();
	pck->Initialize(info.mPath, info.mName, mExt);
	pck->Load();

	std::lock_guard<std::mutex> lock(mMutex);
	mPackages.push_back(pck);
}

void eContent::Load(std::vector<ePackageInfo>& packs)
{
	const uint cpus = eSystem::GetProcessorCount();

	std::vector<std::thread> threads;

	std::vector<ePackageInfo> others;
	uint threadCnt = 0;
	for (const auto& it : packs)
	{
		if (threadCnt > cpus)
		{
			others.push_back(it);
			continue;
		}
		threads.push_back(std::thread(&eContent::LoadPckMT, this, it));
		threadCnt++;
	}

	for (auto& it : threads)
		it.join();

	if (!others.empty())
		Load(others);
}

//! Load packages using only one CPU
void eContent::LoadST(std::vector<ePackageInfo>& packs)
{
	for (auto& it : packs)
		LoadPck(it.mPath, it.mName);
}