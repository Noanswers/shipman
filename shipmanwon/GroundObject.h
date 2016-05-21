#pragma once
#include "MyObject.h"

class CGroundObject : public CMyObject
{
public:
	CGroundObject() = default;
	CGroundObject(float _width, float _length, int _slice)
		:width(_width), length(_length), slice(_slice) {};

	CGroundObject(const CGroundObject&) = default;
	~CGroundObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	void	createShader() override;

private:
	void	createGround(float width, float length, int slice);

	void	createTempVertex(std::vector<VertexType>& vec, float width, float length, int slice);
	void	createGroundLeftUpper(std::vector<VertexType>& tempVec, int slice);
	void	createGroundRightLower(std::vector<VertexType>& tempVec, int slice);

	float width = 100.0f;
	float length = 100.0f;

	int slice = 50;
};

