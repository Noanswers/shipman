#include "stdafx.h"
#include "PlayerData.h"

void CPlayerData::setPlayerKeyRight(unsigned int key)
{
	PlayerKey.right = key;
}

void CPlayerData::setPlayerKeyLeft(unsigned int key)
{
	PlayerKey.left = key;
}

void CPlayerData::setPlayerKeyUp(unsigned int key)
{
	PlayerKey.up = key;
}

void CPlayerData::setPlayerKeyDown(unsigned int key)
{
	PlayerKey.down = key;
}

void CPlayerData::setPlayerKeyStart(unsigned int key)
{
	PlayerKey.start = key;
}

void CPlayerData::setPlayerKeyCancel(unsigned int key)
{
	PlayerKey.cancel = key;
}

void CPlayerData::initialize()
{
	PlayerKey.right = VK_RIGHT;
	PlayerKey.left = VK_LEFT;
	PlayerKey.up = VK_UP;
	PlayerKey.down = VK_DOWN;
	PlayerKey.start = 'a';
	PlayerKey.cancel = 's';
}

//void CPlayerData::setPawnPosition(float x, float y, float z)
//{
//	if (MyPawn == nullptr)
//		return;
//	MyPawn->setTranslate(x, y, z);
//}

KeySetting CPlayerData::getPlayerKeySetting() const
{
	return PlayerKey;
}
