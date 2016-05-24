#include "stdafx.h"
#include "StartScene.h"
#include "MyObject.h"
#include "StageObject.h"
#include "PlayerObject.h"
#include "StartObject.h"
#include "config.h"
#include "SkyObject.h"
#include "SoundManager.h"

void CStartScene::initialize()
{
//	setSceneColor(0.94f, 0.92f, 0.8f, 1.0f);
	
	CStartObject* title = new CStartObject();
	title->setObjectName("title");
	title->setTranslate(0.0f, 1.0f, 0.0f);
	pushBack(title, 10);

	CStartObject* pressButton = new CStartObject();
	pressButton->setObjectName("pressButton");
	pressButton->setTranslate(0.0f, -2.0f, -1.5f);
	pressButton->setScale(0.5f, 0.5f, 0.5f);
	pressButton->setTexture(texturePress);
	pushBack(pressButton, 10);

	CMyObject* sky = new CSkyObject();
	pushBack(sky, 10);

	
}

void CStartScene::doAction(float delta)
{
	auto vector = getObjectsByName("title");
	for (auto& iter : vector)
	{
		iter->setRotate(delta, 0.0f, 0.0f);
	}
}