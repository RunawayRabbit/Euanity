#pragma once

#include <set>
#include <map>

#include "InputDefinitions.h"

struct InputBuffer
{
	std::set<InputOneShot> oneShots;
	std::set<InputToggle> toggles;

	int mouseX;
	int mouseY;
};