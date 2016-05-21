#include "stdafx.h"
#include "InputClass.h"
#include "PlayerData.h"

void CInputClass::initialize()
{
	// Initialize all the keys to being released and not pressed.
	for (int i = 0; i < 256; i++)
	{
		Keys[i] = false;
	}

	return;
}

void CInputClass::keyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	Keys[input] = true;
	return;
}

void CInputClass::keyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	Keys[input] = false;
	return;
}

bool CInputClass::isKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return Keys[key];
}