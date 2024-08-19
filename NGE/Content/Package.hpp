/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef PACKAGE_HPP
#define PACKAGE_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
#include <EDR.hpp>
#include <Map.hpp>
#include "Asset.hpp"
#include <vector>
#include <map>
#include <functional>

//! Asset type functions for construction and destruction
//! \ingroup Content
struct eAssetFncs
{
	std::function<void*(eString, eString, EDR::Archive*)>	mConstr;
	std::function<void(void*)>								mDeconstr;
};

//! Package class for loading and managing assets
//! \ingroup Content
class ePackage : public eBase
{
public:
	void			Initialize(eString path, eString name, eString extension);
	void			Release();

	eMap<eAsset>&	GetCollection();
	eAsset&			GetAsset(eString name);
	bool			FindAsset(eString name);

	void			Load();
	void			LoadAsset(eString name);
	void			ReleaseAsset(eString name);
	void			InsertAsset(eString originalFilePath, eString destName, eString infoFileContent, eString assetType);
	void			RemoveAsset(eString name);

	static void		AddFnc(eString assetType, eAssetFncs& func);
	static void		ClearAllFnc();

	eString&		GetName();

private:
	void			LoadAsset(EDR::Archive* archive, strg name);

	eMap<eAsset>	mAssets;
	eString			mPath;
	eString			mName;
	eString			mExt;
	eString			mTotPath;

	static std::unordered_map<strg, eAssetFncs>	mFncs;
};
#endif