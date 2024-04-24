/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef CONTENT_HPP
#define CONTENT_HPP
#include <Types.hpp>
#include <String.hpp>
#include "Package.hpp"
#include <vector>
#include <mutex>

class eContent
{
public:
												~eContent();
	bool										LoadPck(eString path, eString name);

	//1: path
	//2: name
	bool										Load(std::vector<std::pair<eString, eString>>& packages);

	//	Single thread
	bool										LoadST(std::vector<std::pair<eString, eString>>& packages);
	void										ReleaseAll();
	void										ReleasePck(eString name);
	uint64										GetSize();
	std::vector<eString>						GetPckNames();
	std::vector<std::pair<eString, ePackage*>>* GetPackages();
	ePackage*									GetPackage(eString name);

	//	fileName = path + name + ".pck";
	void										CreatePackage(eString path, eString name);

private:
	void										LoadPckMT(eString path, eString name);

	std::vector<std::pair<eString, ePackage*>>	mPackages;
	std::mutex									mMutex;
};
#endif