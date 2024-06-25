/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Scene.hpp"
#include <Debug.hpp>
#include <EDR.hpp>
#include <Console.hpp>
#include "ComponentFactory.hpp"
#include <algorithm>
#include <sstream>

eScene::~eScene()
{
	Unload();
}

bool eScene::Initialize()
{
	mPaused = false;
	std::vector<eString> toDelete;

	int amount = mActors.size();
	for (int i = 0; i < amount; i++)
	{
		auto& it = mActors[i];
		if (it != nullptr)
		{
			if (!it->Initialize())
			{
				eConsole::PrintLog("ERROR::eGameMap::Initialize", "Actor '" + it->GetName().GetStrg() + "' returned false!");
				eConsole::WriteToDisk();
				return false;
			}
			if (it->ShouldDestroy())
			{
				toDelete.push_back(it->GetName());
				continue;
			}
			it->Awake();
		}
	}

	for (auto& it : toDelete)
		DestroyActor(it);
	toDelete.clear();
	return true;
}

void eScene::Update()
{
	std::vector<eString> toDelete;

	int amount = mActors.size();
	for (int i = 0; i < amount; i++)
	{
		auto& it = mActors[i];
		if (it != nullptr)
		{
			if (mPaused && !it->HasTag("ENGINE#UNPAUSABLE")) continue;
			it->Update();
			if (it->ShouldDestroy())
				toDelete.push_back(it->GetName());
		}
	}

	for (auto& it : toDelete)
		DestroyActor(it, true);
	toDelete.clear();
	ApplySwitch();
}

void eScene::Render()
{
	int amount = mActors.size();
	for (int i = 0; i < amount; i++)
	{
		auto& it = mActors[i];
		if (it != nullptr)
			it->Render();
	}
}

void eScene::Unload(const bool killPersistent)
{
	int amount = mActors.size();
	for (int i = 0; i < amount; i++)
	{
		auto& it = mActors[i];
		if (it != nullptr)
		{
			if (killPersistent && it->IsPersistent())
				continue;

			it->Unload();
			delete it;
		}
	}
	mActors.clear();
	mName.Clear();
}

void eScene::SetName(eString name)
{
	mName.Set(name);
}

void eScene::SetGlobalQuality(eQuality quality)
{
	mGlobalQuality = quality;
}

eQuality eScene::GetGlobalQuality()
{
	return mGlobalQuality;
}

void eScene::Load(eString fileName)
{
	strg filePath = fileName.GetStrg();
	if (!EDR::FileExists(filePath.c_str()))
	{
		eConsole::PrintLog("WARNING::eGameMap::Load", "Map file '" + fileName.GetStrg() + "' could not be found!");
		eConsole::WriteToDisk();
		return;
	}

	SetName(fileName);

	//strg SRC_FILE = EDR::GetContext(filePath, true);
	strg SRC_FILE = EDR::ReadBinary(filePath.c_str());
	EDR::ConvertToText(SRC_FILE);

	int aAmount = EDR::GetIntGI(SRC_FILE, "aa", "info");
	if (aAmount != -1)
	{
		for (uint i = 0; i < (uint)aAmount; i++)
		{
			eActor* rtActor = new eActor();

			strg nickName = EDR::GetStringGI(SRC_FILE, "a" + std::to_string(i), "info");

			strg _nickName = nickName;
			std::replace_if(_nickName.begin(), _nickName.end(), [](char c) { return c == '|'; }, ' ');
			rtActor->SetName(_nickName);

			strg aclass = EDR::GetStringGI(SRC_FILE, "class", nickName);
			std::replace_if(aclass.begin(), aclass.end(), [](char c) { return c == '|'; }, ' ');
			rtActor->SetClass(aclass);

			uint8 detailLvl = static_cast<uint8>(EDR::GetIntGI(SRC_FILE, "detail", nickName));
			if (mGlobalQuality < detailLvl)
			{
				delete rtActor;
				continue;
			}
			rtActor->SetDetailLvl(static_cast<eQuality>(detailLvl));

			const strg grpTag = nickName + "_" + "tags";
			const int tags_total = EDR::GetIntGI(SRC_FILE, "total", grpTag);
			for (int f = 0; f < tags_total; f++)
			{
				strg tag = EDR::GetStringGI(SRC_FILE, "t" + std::to_string(f), grpTag);
				std::replace_if(tag.begin(), tag.end(), [](char c) { return c == '|'; }, ' ');
				rtActor->AddTag(tag);
			}

			int cmpAmount = EDR::GetIntGI(SRC_FILE, "cmps", nickName);
			if (cmpAmount != -1)
			{
				for (uint f = 0; f < (uint)cmpAmount; f++)
				{
					strg cmpName = EDR::GetStringGI(SRC_FILE, "cmp" + std::to_string(f), nickName);
					strg fname = nickName + "_" + cmpName;
					std::replace_if(fname.begin(), fname.end(), [](char c) { return c == ' '; }, '|');
					strg cmpClass = EDR::GetStringGI(SRC_FILE, "class", fname);
					std::replace_if(cmpClass.begin(), cmpClass.end(), [](char c) { return c == '|'; }, ' ');

					if (eComponentFactory::ClassExists(cmpClass))
					{
						strg _cmpName = cmpName;
						std::replace_if(_cmpName.begin(), _cmpName.end(), [](char c) { return c == '|'; }, ' ');
						rtActor->PushComponent(cmpClass, _cmpName);
						uint cmpPos = rtActor->GetComponents()->size() - 1;
						eComponent* currentCmp = rtActor->GetComponents()->at(cmpPos).get();
						uint cmpPrpSize = currentCmp->GetProperties()->size();

						for (uint u = 0; u < cmpPrpSize; u++)
						{
							eProperty& prp = currentCmp->GetProperties()->at(u).mProperty;
							eString prpName = prp.mName;
							strg _prpName = prpName.GetStrg();
							std::replace_if(_prpName.begin(), _prpName.end(), [](char c) { return c == ' '; }, '|');
							switch (prp.mType)
							{
							case ePropertyType::tInt:
							{
								int uvl = EDR::GetIntGI(SRC_FILE, _prpName, fname);
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tUInt:
							{
								uint uvl = static_cast<uint>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tInt8:
							{
								int8 uvl = static_cast<int8>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tUInt8:
							{
								uint8 uvl = static_cast<uint8>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tInt16:
							{
								int16 uvl = static_cast<int16>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tUInt16:
							{
								uint16 uvl = static_cast<uint16>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tInt32:
							{
								uint32 uvl = static_cast<uint32>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tUInt32:
							{
								uint32 uvl = static_cast<uint32>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tInt64:
							{
								int64 uvl = static_cast<int64>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tUInt64:
							{
								uint64 uvl = static_cast<uint64>(EDR::GetIntGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tReal3:
							{
								real3 uvl = static_cast<real3>(EDR::GetNumberGI(SRC_FILE, _prpName, fname));
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tReal6:
							{
								real6 uvl = EDR::GetNumberGI(SRC_FILE, _prpName, fname);
								currentCmp->SetProperty(prpName, uvl);
							} break;
							case ePropertyType::tString:
							{
								strg uvl = EDR::GetStringGI(SRC_FILE, _prpName, fname);
								std::replace_if(uvl.begin(), uvl.end(), [](char c) { return c == '|'; }, ' ');
								currentCmp->SetProperty(prpName, eString(uvl));
							} break;
							case ePropertyType::tVec2:
							{
								strg uvl = EDR::GetStringGI(SRC_FILE, _prpName, fname);
								vec2 rtVl;
								if (uvl.find(";") != strg::npos)
								{
									strg x, y;
									std::stringstream ss(uvl);
									std::getline(ss, x, ';');
									std::getline(ss, y, ';');
									rtVl = vec2(static_cast<real3>(::atof(x.c_str())), static_cast<real3>(::atof(y.c_str())));
								}
								currentCmp->SetProperty(prpName, rtVl);
							} break;
							case ePropertyType::tVec3:
							{
								strg uvl = EDR::GetStringGI(SRC_FILE, _prpName, fname);
								vec3 rtVl;
								if (uvl.find(";") != strg::npos)
								{
									strg x, y, z;
									std::stringstream ss(uvl);
									std::getline(ss, x, ';');
									std::getline(ss, y, ';');
									std::getline(ss, z, ';');
									rtVl = vec3(static_cast<real3>(::atof(x.c_str())), static_cast<real3>(::atof(y.c_str())), static_cast<real3>(::atof(z.c_str())));
								}
								currentCmp->SetProperty(prpName, rtVl);
							} break;
							case ePropertyType::tVec4:
							{
								strg uvl = EDR::GetStringGI(SRC_FILE, _prpName, fname);
								vec4 rtVl;
								if (uvl.find(";") != strg::npos)
								{
									strg x, y, z, w;
									std::stringstream ss(uvl);
									std::getline(ss, x, ';');
									std::getline(ss, y, ';');
									std::getline(ss, z, ';');
									std::getline(ss, w, ';');
									rtVl = vec4(static_cast<real3>(::atof(x.c_str())), static_cast<real3>(::atof(y.c_str())), static_cast<real3>(::atof(z.c_str())), static_cast<real3>(::atof(w.c_str())));
								}
								currentCmp->SetProperty(prpName, rtVl);
							} break;
							case ePropertyType::tBool:
							{
								bool uvl = EDR::GetBoolGI(SRC_FILE, _prpName, fname);
								currentCmp->SetProperty(prpName, uvl);
							} break;
							};
						}
					}
					else
					{
						eConsole::PrintLog("ERROR::eGameMap::Load", "Component '" + cmpClass + "' could not be found!");
						eConsole::WriteToDisk();
						NGE_ASSERT(true);
					}
				}
			}

			mActors.push_back(rtActor);
		}
	}
}

void eScene::Save(eString fileName)
{
	strg filePath = fileName.GetStrg();
	if (EDR::FileExists(filePath.c_str()))
		EDR::RemoveFile(filePath.c_str());

	int aa = mActors.size();
	strg OUT_FILE = "<info>\naa=" + std::to_string(aa) + "\n";

	if (aa != -1)
	{
		for (uint i = 0; i < (uint)aa; i++)
		{
			strg _nname = mActors[i]->GetName().GetStrg();
			std::replace_if(_nname.begin(), _nname.end(), [](char c) { return c == ' '; }, '|');
			OUT_FILE += "a" + std::to_string(i) + "=" + _nname + "\n";
		}
	}
	OUT_FILE += "</info>\n";

	if (aa != -1)
	{
		for (uint i = 0; i < (uint)aa; i++)
		{
			auto& it = mActors[i];

			strg _nname = it->GetName().GetStrg();
			std::replace_if(_nname.begin(), _nname.end(), [](char c) { return c == ' '; }, '|');

			OUT_FILE += "<" + _nname + ">\n";
			int cmpSize = it->GetComponents()->size();

			strg _nclass = it->GetClass().GetStrg();
			std::replace_if(_nclass.begin(), _nclass.end(), [](char c) { return c == ' '; }, '|');
			OUT_FILE += "class=" + _nclass + "\n";

			OUT_FILE += "detail=" + std::to_string(it->GetDetailLvl()) + "\n";

			OUT_FILE += "cmps=" + std::to_string(cmpSize) + "\n";

			if (cmpSize != -1)
			{
				for (uint f = 0; f < (uint)cmpSize; f++)
				{
					strg _cname = it->GetComponents()->at(f)->GetNickName().GetStrg();
					std::replace_if(_cname.begin(), _cname.end(), [](char c) { return c == ' '; }, '|');
					OUT_FILE += "cmp" + std::to_string(f) + "=" + _cname + "\n";
				}
			}
			OUT_FILE += "</" + _nname + ">\n";


			OUT_FILE += "<" + _nname + "_tags>\n";

			auto& tgptr = it->GetTags();
			OUT_FILE += "total=" + std::to_string(tgptr.size()) + "\n";
			for (uint f = 0; f < tgptr.size(); f++)
			{
				eString vl = tgptr[f];
				vl.Replace(' ', '|');
				OUT_FILE += "t" + std::to_string(f) + "=" + vl.GetStrg() + "\n";
			}
			OUT_FILE += "</" + _nname + "_tags>\n";
		}
	}

	if (aa != -1)
	{
		for (uint i = 0; i < (uint)aa; i++)
		{
			auto& it = mActors[i];

			strg _nname = it->GetName().GetStrg();
			std::replace_if(_nname.begin(), _nname.end(), [](char c) { return c == ' '; }, '|');

			int cmpSize = it->GetComponents()->size();
			if (cmpSize != -1)
			{
				for (uint f = 0; f < (uint)cmpSize; f++)
				{
					eComponent* cC = it->GetComponents()->at(f).get();
					if (cC == nullptr)
						NGE_ASSERT(true);

					strg _cname = cC->GetNickName().GetStrg();
					std::replace_if(_cname.begin(), _cname.end(), [](char c) { return c == ' '; }, '|');

					OUT_FILE += "<" + _nname + "_" + _cname + ">\n";

					strg _clnm = cC->GetName().GetStrg();
					std::replace_if(_clnm.begin(), _clnm.end(), [](char c) { return c == ' '; }, '|');

					OUT_FILE += "class=" + _clnm + "\n";

					int prpSize = cC->GetProperties()->size();
					if (prpSize != -1)
					{
						for (uint u = 0; u < (uint)prpSize; u++)
						{
							auto& it2 = cC->GetProperties()->at(u);
							strg prpName = it2.mProperty.mName.GetStrg();
							strg _prpName = prpName;
							std::replace_if(_prpName.begin(), _prpName.end(), [](char c) { return c == ' '; }, '|');

							OUT_FILE += _prpName + "=";

							switch (it2.mProperty.mType)
							{
							case ePropertyType::tInt:
							{
								OUT_FILE += std::to_string(cC->GetProperty<int>(prpName)) + "\n";
							} break;
							case ePropertyType::tUInt:
							{
								OUT_FILE += std::to_string(cC->GetProperty<uint>(prpName)) + "\n";
							} break;
							case ePropertyType::tInt8:
							{
								OUT_FILE += std::to_string(cC->GetProperty<int8>(prpName)) + "\n";
							} break;
							case ePropertyType::tUInt8:
							{
								OUT_FILE += std::to_string(cC->GetProperty<uint8>(prpName)) + "\n";
							} break;
							case ePropertyType::tInt16:
							{
								OUT_FILE += std::to_string(cC->GetProperty<int16>(prpName)) + "\n";
							} break;
							case ePropertyType::tUInt16:
							{
								OUT_FILE += std::to_string(cC->GetProperty<uint16>(prpName)) + "\n";
							} break;
							case ePropertyType::tInt32:
							{
								OUT_FILE += std::to_string(cC->GetProperty<int32>(prpName)) + "\n";
							} break;
							case ePropertyType::tUInt32:
							{
								OUT_FILE += std::to_string(cC->GetProperty<uint32>(prpName)) + "\n";
							} break;
							case ePropertyType::tInt64:
							{
								OUT_FILE += std::to_string(cC->GetProperty<int64>(prpName)) + "\n";
							} break;
							case ePropertyType::tUInt64:
							{
								OUT_FILE += std::to_string(cC->GetProperty<uint64>(prpName)) + "\n";
							} break;
							case ePropertyType::tReal3:
							{
								OUT_FILE += std::to_string(cC->GetProperty<real3>(prpName)) + "\n";
							} break;
							case ePropertyType::tReal6:
							{
								OUT_FILE += std::to_string(cC->GetProperty<real6>(prpName)) + "\n";
							} break;
							case ePropertyType::tString:
							{
								strg _prpstrg = cC->GetProperty<eString>(prpName).GetStrg();
								std::replace_if(_prpstrg.begin(), _prpstrg.end(), [](char c) { return c == ' '; }, '|');
								OUT_FILE += _prpstrg + "\n";
							} break;
							case ePropertyType::tVec2:
							{
								vec2 rtVl = cC->GetProperty<vec2>(prpName);
								OUT_FILE += std::to_string(rtVl.x) + ";" + std::to_string(rtVl.y) + "\n";
							} break;
							case ePropertyType::tVec3:
							{
								vec3 rtVl = cC->GetProperty<vec3>(prpName);
								OUT_FILE += std::to_string(rtVl.x) + ";" + std::to_string(rtVl.y) + ";" + std::to_string(rtVl.z) + "\n";
							} break;
							case ePropertyType::tVec4:
							{
								vec4 rtVl = cC->GetProperty<vec4>(prpName);
								OUT_FILE += std::to_string(rtVl.x) + ";" + std::to_string(rtVl.y) + ";" + std::to_string(rtVl.z) + ";" + std::to_string(rtVl.w) + "\n";
							} break;
							case ePropertyType::tBool:
							{
								bool rtVl = cC->GetProperty<bool>(prpName);
								OUT_FILE += std::to_string(rtVl) + "\n";
							} break;
							};
						}
					}

					OUT_FILE += "</" + _nname + "_" + _cname + ">\n";
				}
			}
		}
	}

	EDR::ConvertToBinary(OUT_FILE);
	EDR::WriteBinary(filePath.c_str(), OUT_FILE);
}

void eScene::SpawnActor(eString fileName, eString name)
{
	//strg SRC_FILE = EDR::GetContext(fileName.GetStrg(), true);
	strg SRC_FILE = EDR::ReadBinary(fileName.Get());
	EDR::ConvertToText(SRC_FILE);

	eActor* rtActor = new eActor();
	rtActor->SetName(name);
	rtActor->SetDetailLvl(eQuality::eVeryLow);

	strg aclass = EDR::GetStringGI(SRC_FILE, "class", "info");
	std::replace_if(aclass.begin(), aclass.end(), [](char c) { return c == '|'; }, ' ');
	rtActor->SetClass(aclass);

	const int tags_total = EDR::GetIntGI(SRC_FILE, "total", "tags");
	for (int i = 0; i < tags_total; i++)
	{
		strg tag = EDR::GetStringGI(SRC_FILE, "t" + std::to_string(i), "tags");
		std::replace_if(tag.begin(), tag.end(), [](char c) { return c == '|'; }, ' ');
		rtActor->AddTag(tag);
	}

	int cmpAmount = EDR::GetIntGI(SRC_FILE, "cmps", "info");
	if (cmpAmount != -1)
	{
		for (uint i = 0; i < (uint)cmpAmount; i++)
		{
			strg cmpName = EDR::GetStringGI(SRC_FILE, "cmp" + std::to_string(i), "info");
			strg cmpClass = EDR::GetStringGI(SRC_FILE, "class", cmpName);
			std::replace_if(cmpClass.begin(), cmpClass.end(), [](char c) { return c == '|'; }, ' ');

			if (eComponentFactory::ClassExists(cmpClass))
			{
				strg _cmpName = cmpName;
				std::replace_if(_cmpName.begin(), _cmpName.end(), [](char c) { return c == '|'; }, ' ');
				rtActor->PushComponent(cmpClass, _cmpName);
				uint cmpPos = rtActor->GetComponents()->size() - 1;
				eComponent* currentCmp = rtActor->GetComponents()->at(cmpPos).get();
				uint cmpPrpSize = currentCmp->GetProperties()->size();

				for (uint i = 0; i < cmpPrpSize; i++)
				{
					eProperty& prp = currentCmp->GetProperties()->at(i).mProperty;
					eString prpName = prp.mName;
					switch (prp.mType)
					{
					case ePropertyType::tInt:
					{
						int uvl = EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName);
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tUInt:
					{
						uint uvl = static_cast<uint>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tInt8:
					{
						int8 uvl = static_cast<int8>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tUInt8:
					{
						uint8 uvl = static_cast<uint8>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tInt16:
					{
						int16 uvl = static_cast<int16>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tUInt16:
					{
						uint16 uvl = static_cast<uint16>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tInt32:
					{
						uint32 uvl = static_cast<uint32>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tUInt32:
					{
						uint32 uvl = static_cast<uint32>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tInt64:
					{
						int64 uvl = static_cast<int64>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tUInt64:
					{
						uint64 uvl = static_cast<uint64>(EDR::GetIntGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tReal3:
					{
						real3 uvl = static_cast<real3>(EDR::GetNumberGI(SRC_FILE, prpName.GetStrg(), cmpName));
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tReal6:
					{
						real6 uvl = EDR::GetNumberGI(SRC_FILE, prpName.GetStrg(), cmpName);
						currentCmp->SetProperty(prpName, uvl);
					} break;
					case ePropertyType::tString:
					{
						strg uvl = EDR::GetStringGI(SRC_FILE, prpName.GetStrg(), cmpName);
						std::replace_if(uvl.begin(), uvl.end(), [](char c) { return c == '|'; }, ' ');
						currentCmp->SetProperty(prpName, eString(uvl));
					} break;
					case ePropertyType::tVec2:
					{
						strg uvl = EDR::GetStringGI(SRC_FILE, prpName.GetStrg(), cmpName);
						vec2 rtVl;
						if (uvl.find(";") != strg::npos)
						{
							strg x, y;
							std::stringstream ss(uvl);
							std::getline(ss, x, ';');
							std::getline(ss, y, ';');
							rtVl = vec2(static_cast<real3>(::atof(x.c_str())), static_cast<real3>(::atof(y.c_str())));
						}
						currentCmp->SetProperty(prpName, rtVl);
					} break;
					case ePropertyType::tVec3:
					{
						strg uvl = EDR::GetStringGI(SRC_FILE, prpName.GetStrg(), cmpName);
						vec3 rtVl;
						if (uvl.find(";") != strg::npos)
						{
							strg x, y, z;
							std::stringstream ss(uvl);
							std::getline(ss, x, ';');
							std::getline(ss, y, ';');
							std::getline(ss, z, ';');
							rtVl = vec3(static_cast<real3>(::atof(x.c_str())), static_cast<real3>(::atof(y.c_str())), static_cast<real3>(::atof(z.c_str())));
						}
						currentCmp->SetProperty(prpName, rtVl);
					} break;
					case ePropertyType::tVec4:
					{
						strg uvl = EDR::GetStringGI(SRC_FILE, prpName.GetStrg(), cmpName);
						vec4 rtVl;
						if (uvl.find(";") != strg::npos)
						{
							strg x, y, z, w;
							std::stringstream ss(uvl);
							std::getline(ss, x, ';');
							std::getline(ss, y, ';');
							std::getline(ss, z, ';');
							std::getline(ss, w, ';');
							rtVl = vec4(static_cast<real3>(::atof(x.c_str())), static_cast<real3>(::atof(y.c_str())), static_cast<real3>(::atof(z.c_str())), static_cast<real3>(::atof(w.c_str())));
						}
						currentCmp->SetProperty(prpName, rtVl);
					} break;
					case ePropertyType::tBool:
					{
						bool uvl = EDR::GetBoolGI(SRC_FILE, prpName.GetStrg(), cmpName);
						currentCmp->SetProperty(prpName, uvl);
					} break;
					};
				}
			}
			else
			{
				eConsole::PrintLog("ERROR::eGameMap::SpawnActor", "Component '" + cmpClass + "' could not be found!");
				eConsole::WriteToDisk();
				NGE_ASSERT(true);
			}
		}
	}
	mActors.push_back(rtActor);
}

void eScene::DestroyActor(eString name, bool forceNow)
{
	for (uint i = 0; i < mActors.size(); i++)
	{
		auto& it = mActors[i];
		if (it != nullptr)
		{
			if (it->GetName().Compare(name))
			{
				if (forceNow)
				{
					it->Unload();
					delete it;
					mActors.erase(mActors.begin() + i);
				}
				else
					it->Destroy();
				return;
			}
		}
	}
	eConsole::PrintLog("WARNING::eGameMap::DestroyActor", "Actor '" + name.GetStrg() + "' could not be found!");
}

eActor* eScene::GetActor(eString name)
{
	for (auto& it : mActors)
	{
		if (it != nullptr)
		{
			if (it->GetName().Compare(name))
				return it;
		}
	}
	return nullptr;
}

std::vector<eActor*>* eScene::GetActors()
{
	return &mActors;
}

void eScene::SwitchScene(eString fileName)
{
	mShouldSwitchScene	= true;
	mSwitchSceneTo		= fileName;
}

void eScene::CancelSwitch()
{
	mShouldSwitchScene = false;
	mSwitchSceneTo.Clear();
}

void eScene::ApplySwitch()
{
	if (mShouldSwitchScene)
	{
		mShouldSwitchScene = false;
		Unload(false);
		Load(mSwitchSceneTo);
		if (!Initialize())
		{
			eConsole::PrintLog("ERROR::eGameMap::Update", "Failed to switch map to '" + mSwitchSceneTo.GetStrg() + "'!");
			eConsole::WriteToDisk();
			NGE_ASSERT(true);
		}
		mSwitchSceneTo.Clear();
	}
}

void eScene::Pause()
{
	mPaused = true;
}

void eScene::Resume()
{
	mPaused = false;
}

bool eScene::IsPaused()
{
	return mPaused;
}