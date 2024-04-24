/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef PACKAGE_HPP
#define PACKAGE_HPP
#include <Types.hpp>
#include <String.hpp>
#include <EDR.hpp>
#include "Asset.hpp"
#include <vector>
#include <map>
#include <functional>

struct eAssetFncs
{
	std::function<void*(eString, eString, EDR::Archive*)>	mConstr;
	std::function<void(void*)>								mDeconstr;
};

struct eFileInfo
{
	eString mPath;	//	Original file
	eString mName;	//	Destination file
};

class ePackage
{
public:
							~ePackage();

	//	fullPath = path + name + ".pck";
	bool					Load(eString path, eString name);
	std::vector<eAsset>*	GetAssets();
	eAsset&					GetAsset(eString name);
	void					ReleaseAsset(eString name);
	void					ReleaseAssets();

	//	fullPath = name + ".pck";
	void					LoadAsset(eString package, eString name);

	//	fullPath = pckPath + pckName + ".pck";
	//	fileName + ".rd";
	void					InsertAsset(eString pckPath, eString pckName, std::vector<eFileInfo>& filePaths, eString fileName, eString infoFileContent, eString assetType);

	static void				AddFnc(eString assetType, eAssetFncs& func);
	static void				ClearAllFnc();

private:
	void					LoadAsset(EDR::Archive* archive, strg name);

	std::vector<eAsset>							mAssets;
	static std::unordered_map<strg, eAssetFncs>	mFncs;
};
#endif