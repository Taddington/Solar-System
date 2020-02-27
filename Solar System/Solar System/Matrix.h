#pragma once
#include "Object.h"
using namespace DirectX;
using namespace std;

namespace Matrix
{
	XMMATRIX view = XMMatrixTranslation(0, 15, -30);
	XMMATRIX target = XMMatrixTranslation(0, 3, 0);
	XMMATRIX viewer;

	XMMATRIX look_at(XMVECTOR& V, XMVECTOR& T, XMVECTOR UP)
	{
		XMVECTOR zAxis = XMVector4Normalize(XMVectorSubtract(T, V));
		XMVECTOR xAxis = XMVector4Normalize(XMVector3Cross(UP, zAxis));
		XMVECTOR yAxis = XMVector4Normalize(XMVector3Cross(zAxis, xAxis));
		XMMATRIX Mat;
		Mat.r[0] = xAxis;
		Mat.r[1] = yAxis;
		Mat.r[2] = zAxis;
		Mat.r[3] = V;
		return Mat;
	}

	void GetInputForCamera()
	{
		float zLocal = 0.0f;
		float xLocal = 0.0f;
		if (GetAsyncKeyState('W'))
			zLocal += 0.1f;
		if (GetAsyncKeyState('S'))
			zLocal -= 0.1f;
		if (GetAsyncKeyState('A'))
			xLocal -= 0.1f;
		if (GetAsyncKeyState('D'))
			xLocal += 0.1f;

		float yRot = 0.0f;
		if (GetAsyncKeyState(VK_LEFT))
			yRot -= 0.01f;
		if (GetAsyncKeyState(VK_RIGHT))
			yRot += 0.01f;

		XMMATRIX translation = XMMatrixTranslation(xLocal, 0.0f, zLocal);
		target = XMMatrixMultiply(translation, target);

		XMMATRIX rotation = XMMatrixRotationY(yRot);
		target = XMMatrixMultiply(rotation, target);


		viewer = XMMatrixMultiply(view, target);
	}
}