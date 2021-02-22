#pragma once

#include <array>

#include "Entity.h"
#include "EntityManager.h"
#include "M3x3.h"

class TransformManager
{
public:
	TransformManager(int capacity);
	~TransformManager();

	bool Get(const Entity entity, M3x3* transform) const;
	bool Set(const Entity entity, const M3x3 transform);
	void Add(const Entity entity, const M3x3 transform);
	void Remove(const Entity entity);
	void GarbageCollect(const EntityManager& entityManager);

private:
	size_t _size;
	size_t _capacity;

	Entity* _entities;
	M3x3* _transforms;

	void* _buffer; // Raw pointer we got back from malloc, where all our shit is

	void Allocate(int newCapacity);
	bool Lookup(const Entity entity, size_t* index) const;
};