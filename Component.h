#pragma once

#include "Entity.h"

using ComponentID = size_t;


class Component
{
public:
	Component( EntityID entity );

	virtual void Awake() {};
	virtual void Update() {};
	virtual void Draw() {};

	virtual ~Component() {};


private:
	const EntityID entityID;

	const ComponentID componentID;
	static ComponentID nextComponent;
};

Component::Component( EntityID entity ) :
	componentID( 1 + nextComponent++ ),
	entityID( entity )
{
}