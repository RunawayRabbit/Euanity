#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <unordered_set>
#include <string>
#include <iostream>

class EntityManager
{
public:
	EntityManager();

	Entity Create();
	bool Exists(Entity entity) const;	
	void Destroy(Entity entity);

	void DEBUGPrintAllEntities()
	{
		for (Entity entity : _entities)
		{
			std::cout << entity.id << " ";
		}
	}

private:
	Entity _nextEntity;
	std::unordered_set<Entity> _entities;
};