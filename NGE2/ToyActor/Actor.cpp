/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "Actor.hpp"
#include <Debug.hpp>
#include <Console.hpp>
#include "Component.hpp"
#include "ComponentFactory.hpp"
#include <EDR.hpp>
#include <algorithm>

bool eActor::Initialize()
{
	mShouldDestroy  = false;
	mPersistent		= false;

	for (auto& it : mComponents)
	{
		if (!it->Initialize())
		{
			eConsole::PrintLog("ERROR::eActor::Initialize", "Component '" + it->GetNickName().GetStrg() + "' inside '" + GetName().GetStrg() + "' Initialization returned false!");
			eConsole::WriteToDisk();
			return false;
		}
		it->Awake();
	}
	return true;
}

void eActor::Update()
{
	if (IsAwake())
	{
		uint amount = mComponents.size();
		for (uint i = 0; i < amount; i++)
		{
			if (i > mComponents.size() - 1)
				continue;

			auto& it = mComponents[i];
			if (it->CanUpdate())
			{
				if (it->IsAwake())
					it->Update();
			}
		}
	}
}

void eActor::Render()
{
	for (auto& it : mComponents)
	{
		if (it->CanRender())
			it->Render();
	}
}

void eActor::Unload()
{
	for (auto& it : mComponents)
	{
		it->Unload();
	}
	mComponents.clear();
}

void eActor::PushComponent(eString name, eString nickName)
{
	if (eComponentFactory::ClassExists(name.GetStrg()))
	{
		for (auto& it : mComponents)
		{
			if (it->GetName().Compare(nickName))
			{
				eConsole::PrintLog("WARNING::eActor::PushComponent", "Component '" + nickName.GetStrg() + "' inside '" + it->GetName().GetStrg() + "' already exists!");
				return;
			}
		}
		mComponents.push_back(std::move(eComponentFactory::CreateObject(name.GetStrg())));
		uint cmpPos = mComponents.size() - 1;
		mComponents[cmpPos]->SetNickName(nickName);
		mComponents[cmpPos]->SetName(name);
		mComponents[cmpPos]->SetParent(this);
	}
	else
		eConsole::PrintLog("WARNING::eActor::PushComponent", "Component Class'" + name.GetStrg() + "' does not exist!");
}

void eActor::PopComponent(eString nickName)
{
	for (uint i = 0; i < mComponents.size(); i++)
	{
		auto& it = mComponents[i];
		if (it->GetNickName().Compare(nickName))
		{
			it->Unload();
			mComponents.erase(mComponents.begin() + i);
			break;
		}
	}
}

std::vector<std::unique_ptr<eComponent>>* eActor::GetComponents()
{
	return &mComponents;
}

eComponent* eActor::GetComponent(eString nickName)
{
	for (auto& it : mComponents)
	{
		if (it.get() != nullptr)
		{
			if (it->GetNickName().Compare(nickName))
				return it.get();
		}
	}
	return nullptr;
}

eComponent* eActor::GetComponentByClass(eString className)
{
	for (auto& it : mComponents)
	{
		if (it.get() != nullptr)
		{
			if (it->GetName().Compare(className))
				return it.get();
		}
	}
	return nullptr;
}

void eActor::SetClass(eString name)
{
	mClassName.Set(name);
}

eString eActor::GetClass()
{
	return mClassName;
}

void eActor::SavePrefab(eString name)
{
	eString OUT_FILE = name;
	eString SRC = "<info>\n";

	strg className = GetClass().GetStrg();
	std::replace_if(className.begin(), className.end(), [](char c) { return c == ' '; }, '|');
	SRC += "class=" + className + "\n";

	SRC += "cmps=" + std::to_string(mComponents.size()) + "\n";
	for (int i = 0; i < mComponents.size(); i++)
	{
		strg cmpName = mComponents[i]->GetNickName().GetStrg();
		std::replace_if(cmpName.begin(), cmpName.end(), [](char c) { return c == ' '; }, '|');
		SRC += "cmp" + std::to_string(i) + "=" + cmpName + "\n";
	}
	SRC += "</info>\n";

	for (auto& it : mComponents)
	{
		strg cmpName = it->GetNickName().GetStrg();
		std::replace_if(cmpName.begin(), cmpName.end(), [](char c) { return c == ' '; }, '|');
		SRC += "<" + cmpName + ">\n";

		strg className = it->GetName().GetStrg();
		std::replace_if(className.begin(), className.end(), [](char c) { return c == ' '; }, '|');
		SRC += "class=" + className + "\n";

		uint cmpPrpSize = it->GetProperties()->size();
		for (uint i = 0; i < cmpPrpSize; i++)
		{
			auto& prp = it->GetProperties()->at(i);

			strg prpName = prp.mProperty.mName.GetStrg();
			std::replace_if(prpName.begin(), prpName.end(), [](char c) { return c == ' '; }, '|');
			SRC += prpName + "=";

			switch (prp.mProperty.mType)
			{
			case ePropertyType::tInt:
			{
				SRC += std::to_string(*reinterpret_cast<int*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tUInt:
			{
				SRC += std::to_string(*reinterpret_cast<uint*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tInt8:
			{
				SRC += std::to_string(*reinterpret_cast<int8*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tUInt8:
			{
				SRC += std::to_string(*reinterpret_cast<uint8*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tInt16:
			{
				SRC += std::to_string(*reinterpret_cast<int16*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tUInt16:
			{
				SRC += std::to_string(*reinterpret_cast<uint16*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tInt32:
			{
				SRC += std::to_string(*reinterpret_cast<int32*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tUInt32:
			{
				SRC += std::to_string(*reinterpret_cast<uint32*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tInt64:
			{
				SRC += std::to_string(*reinterpret_cast<int64*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tUInt64:
			{
				SRC += std::to_string(*reinterpret_cast<uint64*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tReal3:
			{
				SRC += std::to_string(*reinterpret_cast<real3*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tReal6:
			{
				SRC += std::to_string(*reinterpret_cast<real6*>(prp.mProperty.mValue)) + "\n";
			} break;
			case ePropertyType::tString:
			{
				strg prpMsg = reinterpret_cast<eString*>(prp.mProperty.mValue)->GetStrg();
				std::replace_if(prpMsg.begin(), prpMsg.end(), [](char c) { return c == ' '; }, '|');

				SRC += prpMsg + "\n";
			} break;
			case ePropertyType::tVec2:
			{
				vec2 rtVl = *reinterpret_cast<vec2*>(prp.mProperty.mValue);
				strg x = std::to_string(rtVl.x);
				strg y = std::to_string(rtVl.y);
				SRC += x + ";" + y + "\n";
			} break;
			case ePropertyType::tVec3:
			{
				vec3 rtVl = *reinterpret_cast<vec3*>(prp.mProperty.mValue);
				strg x = std::to_string(rtVl.x);
				strg y = std::to_string(rtVl.y);
				strg z = std::to_string(rtVl.z);
				SRC += x + ";" + y + ";" + z + "\n";
			} break;
			case ePropertyType::tVec4:
			{
				vec4 rtVl = *reinterpret_cast<vec4*>(prp.mProperty.mValue);
				strg x = std::to_string(rtVl.x);
				strg y = std::to_string(rtVl.y);
				strg z = std::to_string(rtVl.z);
				strg w = std::to_string(rtVl.w);
				SRC += x + ";" + y + ";" + z + ";" + w + "\n";
			} break;
			case ePropertyType::tBool:
			{
				SRC += std::to_string(*reinterpret_cast<bool*>(prp.mProperty.mValue)) + "\n";
			} break;
			};
		}

		SRC += "</" + cmpName + ">\n";
	}

	if (EDR::FileExists(OUT_FILE.Get()))
		EDR::RemoveFile(OUT_FILE.Get());
	strg out_text = SRC.GetStrg();
	EDR::ConvertToBinary(out_text);
	EDR::WriteBinary(OUT_FILE.Get(), out_text);
	//EDR::WriteFile(OUT_FILE.Get(), out_text);
}

void eActor::SetDetailLvl(eQuality lvl)
{
	mDetailLvl = lvl;
}

eQuality eActor::GetDetailLvl()
{
	return mDetailLvl;
}

void eActor::Destroy()
{
	mShouldDestroy = true;
}

bool eActor::ShouldDestroy()
{
	return mShouldDestroy;
}

void eActor::SetPersistent(bool vl)
{
	mPersistent = vl;
}

bool eActor::IsPersistent()
{
	return mPersistent;
}