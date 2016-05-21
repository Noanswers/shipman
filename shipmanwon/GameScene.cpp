#include "stdafx.h"
#include "GameScene.h"
#include "StageObject.h"
#include "SkyObject.h"
#include "TreeObject.h"

void CGameScene::initialize()
{
	setSceneColor(0.6f, 1.0f, 0.6f, 1.0f);
	//setSceneColor(0.0f, 0.28f, 0.67f, 1.0f); //cobalt blue

	CStageObject* stage = new CStageObject();
	pushBack(stage, 10);

	CMyObject* sky = new CSkyObject();
	pushBack(sky, 10);

//	test code
	CMyObject* tree = new CTreeObject();
	tree->setTranslate(15.0f, -20.0f, 10.0f);
	pushBack(tree, 10);

	CMyObject* tree2 = new CTreeObject();
	tree2->setTranslate(14.0f, -20.0f, 15.0f);
	tree2->setRotate(0.0f, 15.0f, 0.0f);
	pushBack(tree2, 10);

	CMyObject* tree3 = new CTreeObject();
	tree3->setTranslate(15.0f, -20.0f, 22.0f);
	pushBack(tree3, 10);

	CMyObject* tree4 = new CTreeObject();
	tree4->setTranslate(10.0f, -20.0f, 18.0f);
	pushBack(tree4, 10);
}