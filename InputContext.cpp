
// Used to catch out-of-range from std::map.
#include <stdexcept> 

#include "InputContext.h"

InputContext::InputContext()
{
	// @STUB:
}

InputContext::~InputContext()
{
	// @STUB:
}

InputOneShot InputContext::ContainsOneShot(const SDL_Keycode key) const
{
	// Icky horrible copy/pasted exceptions.
	try
	{
		return _oneShots.at(key);
	}
	catch (const std::out_of_range&)
	{
		return InputOneShot::NONE;
	}
}

InputToggle InputContext::ContainsToggle(SDL_Keycode key) const
{
	// Icky horrible copy/pasted exceptions.
	try
	{
		return _toggles.at(key);
	}
	catch (const std::out_of_range&)
	{
		return InputToggle::NONE;
	}
}


void InputContext::AddOneShot(const SDL_Keycode key, InputOneShot oneShot)
{
	_oneShots.insert(std::pair<SDL_Keycode, InputOneShot>(key, oneShot));
}
void InputContext::AddToggle(const SDL_Keycode key, InputToggle toggle)
{
	_toggles.insert(std::pair<SDL_Keycode, InputToggle>(key, toggle));
}