#include "stdafx.h"
#include "InputClass.h"

void CInputClass::initialize()
{
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i<256; i++)
	{
		Keys[i] = false;
	}

	return;


}

void CInputClass::keyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	Keys[input] = true;
	downkeySet(input);
	return;
}

void CInputClass::keyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	Keys[input] = false;
	downkeySet(input);
	return;
}

bool CInputClass::isKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return Keys[key];
}

void CInputClass::downkeyUnset(unsigned int unset)
{
	for (auto i = downkey.begin(); i != downkey.end())
		if ((*i) == unset)
			i = downkey.erase(i);
		else
			++i;
}