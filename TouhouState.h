#pragma once

#include "EntityManager.h"
#include "TransformManager.h"
#include "State.h"


struct TouhouState : public RenderState
{
	EntityManager* entities;
	TransformManager* transforms;

	int picX;
	int picY;
};
