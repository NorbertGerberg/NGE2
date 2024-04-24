/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Package.hpp"
#include <Debug.hpp>
#include <Console.hpp>
#include <algorithm>

std::unordered_map<strg, eAssetFncs> ePackage::mFncs;

ePackage::~ePackage()
{
	ReleaseAssets();
}

std::vector<eAsset>* ePackage::GetAssets()
{
	return &mAssets;
}

eAsset& ePackage::GetAsset(eString name)
{
	for (auto& it : mAssets)
	{
		if (it.mName.Compare(name))
			return it;
	}

	eAsset rtVl;
	return rtVl;
}

void ePackage::AddFnc(eString assetType, eAssetFncs& func)
{
	mFncs[assetType.GetStrg()] = func;
}

void ePackage::ClearAllFnc()
{
	mFncs.clear();
}

bool ePackage::Load(eString path, eString name)
{
	strg fullPath = path.GetStrg() + name.GetStrg() + ".pck";
	EDR::Archive* archive = EDR::OpenArchive(fullPath.c_str());

	if (archive == nullptr)
	{
		eConsole::PrintLog("WARNING::ePackage::Load", "Package '" + name.GetStrg() + "' could not be found!");
		eConsole::WriteToDisk();
		return false;
	}

	strg info = EDR::CopyArchiveFileContents(archive, "info.rd");
	int amount = EDR::GroupItemAmountI(info, name.GetStrg());

	for (int i = 0; i < amount; i++)
	{
		strg group = EDR::GroupItemNameI(info, name.GetStrg(), i);
		LoadAsset(archive, group);
	}
	archive = EDR::CloseArchive(archive);
	return true;
}

void ePackage::LoadAsset(EDR::Archive* archive, strg name)
{
	if (!GetAsset(name).mName.Compare(""))
		return;

	strg infoFile		= name + ".rd";
	strg infoText		= EDR::CopyArchiveFileContents(archive, infoFile.c_str());
	eString assetType	= EDR::GetStringI(infoText, "type");
	assetType.Replace('|', ' ');

	for (auto& it : mFncs)
	{
		if (assetType.Compare(it.first))
		{
			eAsset rtVl;
			rtVl.mName	= name;
			rtVl.mType	= assetType;
			rtVl.mValue = it.second.mConstr(infoText, name, archive);
			mAssets.push_back(rtVl);
			break;
		}
	}
}

void ePackage::ReleaseAsset(eString name)
{
	for (uint i = 0; i < mAssets.size(); i++)
	{
		auto& asset = mAssets[i];
		if (!asset.mName.Compare(name))
			continue;

		for (auto& it : mFncs)
		{
			if (asset.mType.Compare(it.first))
			{
				it.second.mDeconstr(asset.mValue);
				asset.mName.Clear();
				asset.mType.Clear();
				mAssets.erase(mAssets.begin() + i);
				break;
			}
		}
	}
}

void ePackage::ReleaseAssets()
{
	for (auto& it : mAssets)
	{
		for (auto& it2 : mFncs)
		{
			if (it.mType.Compare(it2.first))
			{
				it2.second.mDeconstr(it.mValue);
				it.mName.Clear();
				it.mType.Clear();
			}
		}
	}
	mAssets.clear();
}

void ePackage::LoadAsset(eString package, eString name)
{
	strg fullPath = package.GetStrg() + ".pck";
	EDR::Archive* archive = EDR::OpenArchive(fullPath.c_str());
	NGE_ASSERT(archive != nullptr);
	LoadAsset(archive, name.GetStrg());
	archive = EDR::CloseArchive(archive);
}

void ePackage::InsertAsset(eString pckPath, eString pckName, std::vector<eFileInfo>& filePaths, eString fileName, eString infoFileContent, eString assetType)
{
	strg zipPath = pckPath.GetStrg() + pckName.GetStrg() + ".pck";
	EDR::Archive* archive = EDR::OpenArchive(zipPath.c_str());

	for (auto& it : filePaths)
		EDR::CopyFileToArchive(archive, it.mName.Get(), it.mPath.Get());

	eString assType = assetType;
	assType.Replace(' ', '|');

	strg infoCont = "type=" + assType.GetStrg() + "\n";
	infoCont += infoFileContent.GetStrg();

	EDR::AddFileToArchive(archive, strg(fileName.GetStrg() + ".rd").c_str(), infoCont.c_str());

	strg out_inf = "<" + pckName.GetStrg() + ">\n";
	for (auto& it : mAssets)
		out_inf += "<" + it.mName.GetStrg() + ">\n";
	out_inf += "<" + fileName.GetStrg() + ">\n";
	out_inf += "</" + pckName.GetStrg() + ">\n";
	EDR::RemoveFileFromArchive(archive, "info.rd");
	EDR::AddFileToArchive(archive, "info.rd", out_inf.c_str());

	archive = EDR::CloseArchive(archive);

	LoadAsset(pckPath + pckName, fileName);
}