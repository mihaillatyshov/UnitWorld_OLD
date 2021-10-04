#pragma once

#include <unordered_map>
#include <memory>
#include <iostream>

#include "Core/Texture.h"
#include "Debug/ConsoleLog.h"

#include "Core/Texture.h"
#include "Core/Material.h"

template<class T>
class Manager
{
	//template<class T>
	//Manager() = default;
	//virtual ~Manager() = default;
public:
	constexpr inline void Add(std::string_view name, std::shared_ptr<T> resource) // inline?
	{
#ifdef DEBUG
		if (m_Resource.find(name.data()) == m_Resource.end())
		{
			LOGW("Resource already added: ", typeid(T).name());
		}
#endif
		m_Resource[name.data()] = resource;
	}

	constexpr inline std::shared_ptr<T> Get(std::string_view name) 
	{
#ifdef DEBUG
		if (m_Resource.find(name.data()) == m_Resource.end())
		{
			LOGE("No resource in Manager: ", typeid(T).name());
			return nullptr;
		}
#endif
		return m_Resource[name.data()]; 
	}

	constexpr inline void Remove(std::string_view name)
	{
		#ifdef DEBUG
		if (m_Resource.find(name.data()) == m_Resource.end())
		{
			LOGE("No resource in Manager: ", typeid(T).name());
			return;
		}
#endif
		m_Resource.erase(m_Resource.find(name.data()));
	}
protected:
	std::unordered_map<std::string, std::shared_ptr<T>> m_Resource;
};

class Managers
{
public:
	inline static Manager<Texture> Texture;
	inline static Manager<Material> Material;
};