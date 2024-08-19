/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Package.hpp"
#include <Debug.hpp>
#include <Console.hpp>
#include <algorithm>

std::unordered_map<strg, eAssetFncs> ePackage::mFncs;

void ePackage::Initialize(eString path, eString name, eString extension)
{
	mPath = path;
	mName = name;
	mExt = "." + extension.GetStrg();

	strg tolDir = strg(mPath.GetStrg() + mName.GetStrg() + mExt.GetStrg());
	if (!EDR::FileExists(tolDir.c_str()))
	{
		EDR::Archive* archive = EDR::CreateArchive(tolDir.c_str());
		strg infFile = "<" + name.GetStrg() + ">\n";
		infFile += "</" + name.GetStrg() + ">\n";
		EDR::AddFileToArchive(archive, "info.rd", infFile.c_str());
		archive = EDR::CloseArchive(archive);
	}
	mTotPath = tolDir;
}

void ePackage::Release()
{
	for (auto& it : mAssets.Geta())
	{
		for (auto& it2 : mFncs)
		{
			if (it.second.mType.Compare(it2.first))
			{
				it2.second.mDeconstr(it.second.mValue);
				it.first.Clear();
				it.second.mType.Clear();
			}
		}
	}
	mAssets.Popa();
	mPath.Clear();
}

eMap<eAsset>& ePackage::GetCollection()
{
	return mAssets;
}

eAsset& ePackage::GetAsset(eString name)
{
	return mAssets.Get(name);
}

void ePackage::Load()
{
	EDR::Archive* archive = EDR::OpenArchive(mTotPath.Get());

	if (archive == nullptr)
	{
		eConsole::PrintLog("WARNING::ePackage::Load", "Package '" + mName.GetStrg() + "' could not be found!");
		eConsole::WriteToDisk();
		return;
	}

	strg info = EDR::CopyArchiveFileContents(archive, "info.rd");
	int amount = EDR::GroupItemAmountI(info, mName.GetStrg());

	for (int i = 0; i < amount; i++)
	{
		strg group = EDR::GroupItemNameI(info, mName.GetStrg(), i);
		LoadAsset(archive, group);
	}
	archive = EDR::CloseArchive(archive);
}

void ePackage::LoadAsset(eString name)
{
	EDR::Archive* archive = EDR::OpenArchive(mTotPath.Get());
	NGE_ASSERT(archive != nullptr);
	LoadAsset(archive, name.GetStrg());
	archive = EDR::CloseArchive(archive);
}

void ePackage::ReleaseAsset(eString name)
{
	bool isValid = false;
	for (uint i = 0; i < mAssets.Size(); i++)
	{
		auto& asset = mAssets[i];
		eString& assName = mAssets.IdAt(i);

		if (!assName.Compare(name))
			continue;

		for (auto& it : mFncs)
		{
			if (asset.mType.Compare(it.first))
			{
				it.second.mDeconstr(asset.mValue);
				asset.mType.Clear();
				isValid = true;
				break;
			}
		}
		if (isValid) break;
	}

	if (isValid)
		mAssets.Pop(name);
}

void ePackage::InsertAsset(eString originalFilePath, eString destName, eString infoFileContent, eString assetType)
{
	EDR::Archive* archive = EDR::OpenArchive(mTotPath.Get());

	strg newName = destName.GetStrg() + ".src";
	EDR::CopyFileToArchive(archive, newName.c_str(), originalFilePath.Get());

	eString assType = assetType;
	assType.Replace(' ', '|');

	strg infoCont = "type=" + assType.GetStrg() + "\n";
	infoCont += infoFileContent.GetStrg();

	EDR::AddFileToArchive(archive, strg(destName.GetStrg() + ".rd").c_str(), infoCont.c_str());
	
	strg out_inf = "<" + mName.GetStrg() + ">\n";
	for(auto& it : mAssets.Geta())
		out_inf += "<" + it.first.GetStrg() + ">\n";
	out_inf += "<" + destName.GetStrg() + ">\n";
	out_inf += "</" + mName.GetStrg() + ">\n";
	EDR::RemoveFileFromArchive(archive, "info.rd");
	EDR::AddFileToArchive(archive, "info.rd", out_inf.c_str());

	archive = EDR::CloseArchive(archive);
	LoadAsset(destName);
}

void ePackage::RemoveAsset(eString name)
{
	ReleaseAsset(name);
	EDR::Archive* archive = EDR::OpenArchive(mTotPath.Get());

	strg infFile = name.GetStrg() + ".rd";
	EDR::RemoveFileFromArchive(archive, infFile.c_str());

	strg srcFile = name.GetStrg() + ".src";
	EDR::RemoveFileFromArchive(archive, srcFile.c_str());

	strg out_inf = "<" + mName.GetStrg() + ">\n";
	for (uint i = 0; i < mAssets.Size(); i++)
		out_inf += "<" + mAssets.IdAt(i).GetStrg() + ">\n";
	out_inf += "</" + mName.GetStrg() + ">\n";
	EDR::RemoveFileFromArchive(archive, "info.rd");
	EDR::AddFileToArchive(archive, "info.rd", out_inf.c_str());

	archive = EDR::CloseArchive(archive);
}

void ePackage::AddFnc(eString assetType, eAssetFncs& func)
{
	mFncs[assetType.GetStrg()] = func;
}

void ePackage::ClearAllFnc()
{
	mFncs.clear();
}

void ePackage::LoadAsset(EDR::Archive* archive, strg name)
{
	if (mAssets.Find(name)) return;

	strg infoFile = name + ".rd";
	strg infoText = EDR::CopyArchiveFileContents(archive, infoFile.c_str());
	eString assetType = EDR::GetStringI(infoText, "type");
	assetType.Replace('|', ' ');

	for (auto& it : mFncs)
	{
		if (assetType.Compare(it.first))
		{
			eAsset rtVl;
			rtVl.mType = assetType;
			rtVl.mValue = it.second.mConstr(infoText, name, archive);
			mAssets.Push(rtVl, name);
			break;
		}
	}
}

eString& ePackage::GetName()
{
	return mName;
}

bool ePackage::FindAsset(eString name)
{
	return mAssets.Find(name);
}