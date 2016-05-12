#include "stdafx.h"
#include "StartScene.h"
#include "MyObject.h"

void CStartScene::initialize()
{
	setSceneColor(0.0f, 0.0f, 0.0f, 1.0f);

	CMyObject* object1 = new CMyObject();
	pushBack(object1, 10);

	object1->setTranslate(3.0f, 0.0f, 0.0f);
	object1->setScale(0.5f, 0.5f, 0.5f);
}