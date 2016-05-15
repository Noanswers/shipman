#include "stdafx.h"
#include "StartScene.h"
#include "MyObject.h"
#include "StageObject.h"

void CStartScene::initialize()
{
	setSceneColor(0.0f, 0.0f, 0.0f, 1.0f);

	CStageObject* stage = new CStageObject();
	//CMyObject* stage = new CMyObject();
	//CMyObject* stage = new CStageObject; //test

	pushBack(stage, 10);

	//object1->setTranslate(3.0f, 0.0f, 0.0f);
	//object1->setScale(0.5f, 0.5f, 0.5f);
}