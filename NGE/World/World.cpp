/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "World.hpp"
#include "Module.hpp"
#include <VersionInfo.hpp>
#include <Console.hpp>
#include "StandardProperties.hpp"

eFactory<eDoer>		eWorld::mFactory;
eMap<ePropertyType>	eWorld::mPropTypes;

bool eWorld::Initialize()
{
	bool rtVl = true;

	int amount = mDoers.Size();
	for (int i = 0; i < amount; i++)
	{
		eDoer* it2 = mDoers[i];
		for (auto& it3 : it2->GetModules().Geta())
		{
			auto& it4 = it3.second;
			it4->SetParent(it2);
			if (!it4->Initialize())
			{
				rtVl = false;
				eConsole::PrintLog("ERROR::eWorld::Initialize", "Module '" + it3.first.GetStrg() + "' inside '" + mDoers.IdAt(i).GetStrg() + "' returned false!");
				eConsole::WriteToDisk();
			}
			it4->Awake();
		}
		it2->SetWorld(this);
		if (!it2->Initialize())
		{
			rtVl = false;
			eConsole::PrintLog("ERROR::eWorld::Initialize", "Doer '" + mDoers.IdAt(i).GetStrg() + "' returned false!");
			eConsole::WriteToDisk();
		}
		it2->Awake();
	}
	return rtVl;
}

void eWorld::Update()
{
	std::vector<eString> toDestroy;

	int amount = mDoers.Size();
	for (int i = 0; i < amount; i++)
	{
		eDoer* it2 = mDoers[i];
		if (it2->IsAwake() && it2->bCanUpdate)
		{
			for (auto& it3 : it2->GetModules().Geta())
			{
				auto& it4 = it3.second;
				if (it4->IsAwake() && it4->bCanUpdate)
				{
					it4->Alarm().Update();
					it4->Update();
				}
			}
			it2->Alarm().Update();
			it2->Update();

			if (it2->ShouldDestroy())
				toDestroy.push_back(mDoers.IdAt(i));
		}
	}

	for (auto& it : toDestroy)
		mDoers.Pop(it);
}

void eWorld::Render()
{
	int amount = mDoers.Size();
	for (int i = 0; i < amount; i++)
	{
		eDoer* it2 = mDoers[i];
		for (auto& it3 : it2->GetModules().Geta())
		{
			auto& it4 = it3.second;
			if (it4->bCanRender) it4->Render();
		}
		if (it2->bCanRender) it2->Render();
	}
}

void eWorld::Unload()
{
	mDoers.Popa();
}

eDoer* eWorld::GetDoer(eString name)
{
	if (!mDoers.Find(name)) return nullptr;
	return mDoers[name];
}

eFactory<eDoer>& eWorld::Factory()
{
	return mFactory;
}

void eWorld::Save(eString outfile)
{
	mFilepath = outfile;

	if (EDR::FileExists(outfile.Get()))
		EDR::RemoveFile(outfile.Get());

	EDR::Archive* archive = EDR::CreateArchive(outfile.Get());

		// Info file
		eString infOut = eTEXT("NGE WORLD FILE\n");
		infOut += eTEXT("EngineVersion=") + eString::ToString(NGE_ENGINE_INFO_VERSION) + eTEXT("\n");
		infOut += eTEXT("Compiler=") + eTEXT(NGE_COMPILER_NAME) + eTEXT("\nCompilerVersion=") + eString::ToString(NGE_COMPILER_VERSION) + eTEXT("\n");

		infOut += "<DOERS>\n";
		for (auto& it : mDoers.Geta())
		{
			eString name = it.first;
			name.Replace(' ', '|');
			infOut += eTEXT("<") + name + eTEXT(">\n");
		}
		infOut += "</DOERS>\n";

		infOut += OnSave_info();

		strg OUTME = infOut.GetStrg();
		OUTME = EDR::WriteBinaryToString(OUTME);
		EDR::AddBinaryToArchive(archive, "info", OUTME.c_str(), OUTME.size());

		// Doers
		std::vector<strg> TMPSVSTOR;
		for (auto& it : mDoers.Geta())
		{
			auto& obj = it.second;

			eString name = it.first;
			name.Replace(' ', '|');

			eString objclass = obj->mClassName;
			objclass.Replace(' ', '|');
			eString OUT = eTEXT("class=") + objclass + eTEXT("\n");

			OUT += eTEXT("<PROPS>\n");
			for (auto& prop : obj->Properties().Geta().Geta())
			{
				if (!mPropTypes.Find(prop.second.mType))
				{
					eConsole::PrintLog("WARNING::eWorld::Save", "Property type '" + prop.second.mType.GetStrg() + "' was not registered!");
					continue;
				}
				eString pname = prop.first;
				pname.Replace(' ', '|');
				OUT += pname + eTEXT("=") + mPropTypes[prop.second.mType].mOnSave(prop.second.mValue) + eTEXT("\n");
			}
			OUT += eTEXT("</PROPS>\n");

			OUT += eTEXT("<TAGS>\n");
			const uint64 tsize = obj->Tags().GetCollection().size();
			OUT += eTEXT("a=") + eString::ToString(tsize) + eTEXT("\n");
			for(uint64 i = 0; i < tsize; i++)
			{
				auto& tag = obj->Tags().GetCollection()[i];
				eString fintag = tag;
				fintag.Replace(' ', '|');
				OUT += eTEXT("t") + eString::ToString(i) + eTEXT("=") + fintag + eTEXT("\n");
			}
			OUT += eTEXT("</TAGS>\n");

			for (auto& it : obj->GetModules().Geta())
			{
				eString modname = it.first;
				modname.Replace(' ', '|');
				OUT += eTEXT("<") + modname + eTEXT(">\n");
				for (auto& prop : it.second->Properties().Geta().Geta())
				{
					if (!mPropTypes.Find(prop.second.mType))
					{
						eConsole::PrintLog("WARNING::eWorld::Save", "Property type '" + prop.second.mType.GetStrg() + "' was not registered!");
						continue;
					}
					eString pname = prop.first;
					pname.Replace(' ', '|');
					OUT += pname + eTEXT("=") + mPropTypes[prop.second.mType].mOnSave(prop.second.mValue) + eTEXT("\n");
				}
				OUT += eTEXT("</") + modname + eTEXT(">\n");
			}

			TMPSVSTOR.push_back(OUT.GetStrg());
			strg& curstorspc = TMPSVSTOR[TMPSVSTOR.size() - 1];
			curstorspc = EDR::WriteBinaryToString(curstorspc);
			name.Replace('|', '-');
			strg outfname = name.GetStrg() + ".doer";
			EDR::AddBinaryToArchive(archive, outfname.c_str(), curstorspc.c_str(), curstorspc.size());
		}

		OnSave(archive);

	archive = EDR::CloseArchive(archive);
}

void eWorld::Load(eString filepath, const bool forceload)
{
	if (!EDR::FileExists(filepath.Get()))
	{
		eConsole::PrintLog("ERROR::eWorld::Load", "World file '" + filepath.GetStrg() + "' could not be found!");
		return;
	}

	mFilepath = filepath;

	EDR::Archive* archive = EDR::OpenArchive(filepath.Get());

		EDR::ZSTAT _INFO = EDR::GetArchiveFileContents(archive, "info");
		strg INFO(_INFO.c, _INFO.size);
		INFO = EDR::ReadBinaryToString(INFO);
		delete[] _INFO.c;

		const uint16 INFENV = static_cast<uint16>(EDR::GetIntI(INFO, "EngineVersion"));
		if (INFENV != NGE_ENGINE_INFO_VERSION)
		{
			eConsole::PrintLog("WARNING::eWorld::Load", "World was created using a different engine version that may be incompatible!");
			eConsole::PrintLine(eTEXT("World: v") + eString::ToString(INFENV));
			eConsole::PrintLine(eTEXT("Engine: v") + eString::ToString(NGE_ENGINE_INFO_VERSION));
		}

		const strg COMPNAME = EDR::GetStringI(INFO, "Compiler");
		if (COMPNAME != NGE_COMPILER_NAME)
		{
			eConsole::PrintLog("ERROR::eWorld::Load", "Invalid runtime library!");
			eConsole::PrintLine(eTEXT("World: ") + COMPNAME);
			eConsole::PrintLine(eTEXT("Engine: ") + eTEXT(NGE_COMPILER_NAME));
			eConsole::WriteToDisk();
			archive = EDR::CloseArchive(archive);
			return;
		}

		const int COMPV = EDR::GetIntI(INFO, "CompilerVersion");
		if(COMPV != NGE_COMPILER_VERSION)
		{
			eConsole::PrintLog("WARNING::eWorld::Load", "Different compiler version detected!");
			eConsole::PrintLine(eTEXT("World: ") + eString::ToString(COMPV));
			eConsole::PrintLine(eTEXT("Engine: ") + eString::ToString(NGE_COMPILER_VERSION));
		}

		eString _inftmp = INFO;
		OnLoad_info(_inftmp);
		INFO = _inftmp.GetStrg();

		const uint doera = EDR::GroupItemAmountI(INFO, "DOERS");
		for (uint i = 0; i < doera; i++)
		{
			eString doer_name = EDR::GroupItemNameI(INFO, "DOERS", i);
			doer_name.Replace('|', '-');
			EDR::ZSTAT _doer_info = EDR::GetArchiveFileContents(archive, eTEXT(doer_name + ".doer").Get());
			strg doer_info(_doer_info.c, _doer_info.size);
			doer_info = EDR::ReadBinaryToString(doer_info);
			doer_name.Replace('-', ' ');
			delete[] _doer_info.c;

			eString classname = EDR::GetStringI(doer_info, "class");
			classname.Replace('|', ' ');

			if (!mFactory.ClassExists(classname))
			{
				eConsole::PrintLog("ERROR::eWorld::Load", eTEXT("Class '") + classname + eTEXT("' was not registered!"));
				if (!forceload)
				{
					eConsole::WriteToDisk();
					archive = EDR::CloseArchive(archive);
					return;
				}
			}

			mDoers.Push(mFactory.CreateObject(classname), doer_name);
			eDoer* obj = mDoers.Get(doer_name);
			obj->mClassName = classname;

			for (auto& it : obj->Properties().Geta().Geta())
			{
				eString prop_name = it.first;
				prop_name.Replace(' ', '|');
				if (!mPropTypes.Find(it.second.mType))
				{
					eConsole::PrintLog("WARNING::eWorld::Load", "Property type '" + it.second.mType.GetStrg() + "' was not registered!");
					continue;
				}
				mPropTypes[it.second.mType].mOnLoad(it.second.mValue, EDR::GetStringGI(doer_info, prop_name.GetStrg(), "PROPS"));
			}

			const uint taga = static_cast<uint>(EDR::GetIntGI(doer_info, "a", "TAGS"));
			for (uint f = 0; f < taga; f++)
			{
				eString tagvl = EDR::GetStringGI(doer_info, "t" + std::to_string(f), "TAGS");
				tagvl.Replace('|', ' ');
				obj->Tags().Push(tagvl);
			}

			for (auto& it : obj->GetModules().Geta())
			{
				eString mod_name = it.first;
				mod_name.Replace(' ', '|');

				for (auto& prop : it.second->Properties().Geta().Geta())
				{
					if (!mPropTypes.Find(prop.second.mType))
					{
						eConsole::PrintLog("WARNING::eWorld::Load", "Property type '" + prop.second.mType.GetStrg() + "' was not registered!");
						continue;
					}
					eString prop_name = prop.first;
					prop_name.Replace(' ', '|');
					mPropTypes[prop.second.mType].mOnLoad(prop.second.mValue, EDR::GetStringGI(doer_info, prop_name.GetStrg(), mod_name.GetStrg()));
				}
			}
		}
	
		OnLoad(archive);

	archive = EDR::CloseArchive(archive);
}

eString eWorld::OnSave_info()
{
	return eTEXT("");
}

void eWorld::OnSave(EDR::Archive* archive)
{
	return;
}

void eWorld::OnLoad_info(eString& info)
{
	return;
}

void eWorld::OnLoad(EDR::Archive* archive)
{
	return;
}

void eWorld::PopPropertyTypes()
{
	mPropTypes.Popa();
}

eString eWorld::GetFilepath()
{
	return mFilepath;
}

using namespace eStandardProperties;
void eWorld::INIT()
{
	RegisterPropertyType<int>(		{ stp_int,		stp_int_ });
	RegisterPropertyType<int8>(		{ stp_int8,		stp_int8_ });
	RegisterPropertyType<int16>(	{ stp_int16,	stp_int16_ });
	RegisterPropertyType<int32>(	{ stp_int32,	stp_int32_ });
	RegisterPropertyType<int64>(	{ stp_int64,	stp_int64_ });
	RegisterPropertyType<uint>(		{ stp_uint,		stp_uint_ });
	RegisterPropertyType<uint8>(	{ stp_uint8,	stp_uint8_ });
	RegisterPropertyType<uint16>(	{ stp_uint16,	stp_uint16_ });
	RegisterPropertyType<uint32>(	{ stp_uint32,	stp_uint32_ });
	RegisterPropertyType<uint64>(	{ stp_uint64,	stp_uint64_ });
	RegisterPropertyType<real3>(	{ stp_Real3,	stp_Real3_ });
	RegisterPropertyType<real6>(	{ stp_Real6,	stp_Real6_ });
	RegisterPropertyType<bool>(		{ stp_Bool,		stp_Bool_ });
	RegisterPropertyType<vec2>(		{ stp_Vec2,		stp_Vec2_ });
	RegisterPropertyType<vec3>(		{ stp_Vec3,		stp_Vec3_ });
	RegisterPropertyType<vec4>(		{ stp_Vec4,		stp_Vec4_ });
	RegisterPropertyType<eString>(	{ stp_String,	stp_String_ });
}