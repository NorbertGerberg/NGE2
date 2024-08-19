/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef CONTENT_HPP
#define CONTENT_HPP
#include <Types.hpp>
#include <String.hpp>
#include <Base.hpp>
#include "Package.hpp"
#include <vector>
#include <mutex>

//! Package info
//! \ingroup Content
struct ePackageInfo
{
	eString mPath;
	eString mName;
};

//! Content class for loading and managing packages
//! \ingroup Content
class eContent : public eBase
{
public:
		 ~eContent();
	void Initialize(eString extension);
	void Release();
	void ReleasePck(eString name);
	void LoadPck(eString path, eString name);
	void CreatePck(eString path, eString name);
	void Load(std::vector<ePackageInfo>& packs);
	void LoadST(std::vector<ePackageInfo>& packs);

	std::vector<ePackage*>& GetCollection();
	ePackage*				GetPck(eString name);

private:
	void LoadPckMT(ePackageInfo info);

	eString					mExt;
	std::vector<ePackage*>	mPackages;
	std::mutex				mMutex;
};
#endif