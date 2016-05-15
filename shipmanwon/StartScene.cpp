#include "stdafx.h"
#include "StartScene.h"
#include "MyObject.h"
#include "StageObject.h"
#include "PlayerObject.h"

void CStartScene::initialize()
{
	setSceneColor(0.0f, 0.0f, 0.0f, 1.0f);

	//CMyObject* stage = new CMyObject();

	//CStageObject* stage2 = new CStageObject();
	//CMyObject* stage2 = new CStageObject(); //test
	CMyObject* object = new CPlayerObject();

	//pushBack(stage, 10);
	//pushBack(stage2, 10);
	pushBack(object, 10);

	//object1->setTranslate(3.0f, 0.0f, 0.0f);
	//object1->setScale(0.5f, 0.5f, 0.5f);
}