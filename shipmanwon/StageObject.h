#pragma once
#include "MyObject.h"


class CStageObject : public CMyObject
{
public:
	CStageObject() = default;
	CStageObject(float _topRadius, float _bottomRadius, float _height, int _slice)
		:topRadius(_topRadius), bottomRadius(_bottomRadius), height(_height), slice(_slice) {};
	
	CStageObject(const CStageObject&) = default;
	~CStageObject() = default;
	
	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;

	void createShader() override;

private:
	void createStage(float topRadius, float bottomRadius, float height, int slice);

	void createTempVertex(float topRadius, float bottomRadius, float height, int slice, int stack);
	void createUpperTriangleOnStack(int slice);
	void createLowerTriangleOnStack(int slice);

	float topRadius = 4.0f;
	float bottomRadius = 8.0f;

	float height = 10.0f;
	int slice = 15;
	int stack = 1;
};

