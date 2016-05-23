#pragma once
#include "MyObject.h"

class CTreeObject : public CMyObject
{
public:
	CTreeObject() = default;
	CTreeObject(float _upperHeight, float _upperWidth, float _lowerWidthTop, float _lowerWidthBottom, float _lowerHeight, int _slice)
		:upperHeight(_upperHeight), upperWidth(_upperWidth), lowerWidthTop(_lowerWidthTop), lowerWidthBottom(_lowerWidthBottom), lowerHeight(_lowerHeight), slice(_slice) {};
	CTreeObject(const CTreeObject&) = default;
	~CTreeObject() = default;

	bool	initialize(ID3D11Device* device, HWND hWnd) override;
	void	shutdown() override;
	void	createShader() override;

private:
	void	createTree(float upperHeight, float upperWidth, float lowerWidthTop, float lowerWidthBottom, float lowerHeight, int slice);

	void	createTempUpperVertex(float upperHeight, float upperwidth, float lowerHeight, int slice);
	void	createUpperPart(float upperHeight, float upperwidth, float lowerHeight, int slice);

	void	createTempVertexOfWood(float lowerWidthTop, float lowerWidthBottom, float lowerHeight, int slice);

	void	createUpperTriangleOfWood(float lowerWidthTop, float lowerWidthBottom, int slice, int base);
	void	createLowerTriangleOfWood(float lowerWidthTop, float lowerWidthBottom, int slice, int base);

	float upperHeight = 12.0f;
	float upperWidth = 4.0f;
	float slice = 4;

	float lowerWidthTop = 1.0f;
	float lowerWidthBottom = 2.0f;
	float lowerHeight = 1.0f;
};

