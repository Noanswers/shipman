#include "stdafx.h"
#include "StartScene.h"
#include "MyObject.h"
#include "StageObject.h"
#include "PlayerObject.h"
#include "StartObject.h"

void CStartScene::initialize()
{
	setSceneColor(0.2f, 0.0f, 0.0f, 1.0f);


	CStartObject* object = new CStartObject();

	//object->initialize();
	pushBack(object, 10);
	object->setTranslate(0, 0, 0);
	/*CPlayerObject* object = new CPlayerObject();

	pushBack(object, 10);
	object->setScale(0.5f, 0.2f, 0.5f);*/


}