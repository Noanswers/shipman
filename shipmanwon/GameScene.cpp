#include "stdafx.h"
#include "GameScene.h"
#include "StageObject.h"
#include "SkyObject.h"
#include "TreeObject.h"
#include "GroundObject.h"

void CGameScene::initialize()
{
	setSceneColor(0.6f, 1.0f, 0.6f, 1.0f);

	CMyObject* stage = new CStageObject();
	pushBack(stage, 10);

	CMyObject* sky = new CSkyObject();
	pushBack(sky, 10);

	CMyObject* ground = new CGroundObject();
	ground->setTranslate(0.0f, -10.0f, 0.0f);
	pushBack(ground, 10);
	
//	tree ¹èÄ¡
	int radius = 10.0f;
	int maxRadius = 12.0f;
	for (int i = 0; i < 40 ; ++i)
	{
		float alpha = 0.1f*i;
		float theta = fmod((rand()), XM_2PI);
		float delta = (maxRadius - radius) * sinf(fmod((rand()), XM_2PI));
		CMyObject* tree = new CTreeObject(8.0f+ alpha, 3.0f, 1.0f, 2.0f, 1.0f+ alpha/4, 4);

		tree->setTranslate(radius*cos(theta) + delta, -5.0f, radius*sin(theta) + delta);
		tree->setScale(0.5f, 0.5f, 0.5f);
		pushBack(tree, 10);
	}
}