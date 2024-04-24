/*=========================================
	Copyright 2024-2025 Norbert Gerberg
=========================================*/
#ifndef COMPONENTFACTORY_HPP
#define COMPONENTFACTORY_HPP
#include <Types.hpp>
#include <String.hpp>
#include "Component.hpp"
#include <unordered_map>
#include <memory>
#include <type_traits>
#include <functional>

class eComponentFactory
{
public:
	template<typename T>
	static void RegisterType(strg name)
	{
		type_map_[name] = []() -> std::unique_ptr<eComponent> {
			return std::make_unique<T>();
		};
		return;
	}

	static std::unique_ptr<eComponent> CreateObject(strg name);
	static bool ClassExists(strg name);

	static std::unordered_map<strg, std::function<std::unique_ptr<eComponent>()>>& GetMap();
private:
	static std::unordered_map<strg, std::function<std::unique_ptr<eComponent>()>> type_map_;
};
#endif