#include "stdafx.h"
#include "GameScene.h"

void CGameScene::initialize()
{
	setSceneColor(0.2f, 0.2f, 0.2f, 1.0f);

	CMyObject* object1 = new CMyObject();
	pushBack(object1, 10);

	CMyObject* object2 = new CMyObject();
	pushBack(object2, 9);

	CMyObject* object3 = new CMyObject();
	pushBack(object3, 10);

	CMyObject* object4 = new CMyObject();
	pushBack(object4, 10);

	object1->setTranslate(3.0f, 0.0f, 0.0f);
	object1->setScale(0.5f, 0.5f, 0.5f);

	object2->setTranslate(-3.0f, 0.0f, 0.0f);
	object2->setScale(0.5f, 0.5f, 0.5f);

	object3->setTranslate(0.0f, 2.0f, 0.0f);
	object3->setScale(0.5f, 0.5f, 0.5f);
	object3->setColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);

	object4->setTranslate(0.0f, -2.0f, 0.0f);
	object4->setScale(0.5f, 0.5f, 0.5f);
	object4->setColorRGBA(0.2f, 0.2f, 0.2f, 1.0f);
}
