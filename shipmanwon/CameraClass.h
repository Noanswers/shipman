#pragma once

#include <directxmath.h>
using namespace DirectX;

class CCameraClass
{
public:
	CCameraClass()
	{
		m_positionX = 0.0f;
		m_positionY = 0.0f;
		m_positionZ = 0.0f;

		m_rotationX = 0.0f;
		m_rotationY = 0.0f;
		m_rotationZ = 0.0f;
	};

	CCameraClass(const CCameraClass&) = default;
	~CCameraClass() = default;

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
};
