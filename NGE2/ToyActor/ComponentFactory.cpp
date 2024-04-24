/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#include "ComponentFactory.hpp"

std::unordered_map<std::string, std::function<std::unique_ptr<eComponent>()>> eComponentFactory::type_map_;
std::unique_ptr<eComponent> eComponentFactory::CreateObject(strg name)
{
	auto it = type_map_.find(name);
	if (it != type_map_.end())
		return (it->second)();
	return nullptr;
}

bool eComponentFactory::ClassExists(strg name)
{
	auto it = type_map_.find(name);
	if (it != type_map_.end())
		return true;
	return false;
}

std::unordered_map<strg, std::function<std::unique_ptr<eComponent>()>>& eComponentFactory::GetMap()
{
	return type_map_;
}