#include "stdafx.h"
#include "StartScene.h"
#include "MyObject.h"
#include "StageObject.h"
#include "PlayerObject.h"

void CStartScene::initialize()
{
	setSceneColor(0.2f, 0.0f, 0.0f, 1.0f);

	//CMyObject* stage = new CMyObject();

	//CStageObject* stage2 = new CStageObject();
	//CMyObject* stage2 = new CStageObject(); //test
	
	CPlayerObject* object = new CPlayerObject();

	pushBack(object, 10);
	object->setScale(0.5f, 0.2f, 0.5f);

	//object1->setTranslate(3.0f, 0.0f, 0.0f);
	//object1->setScale(0.5f, 0.5f, 0.5f);
}