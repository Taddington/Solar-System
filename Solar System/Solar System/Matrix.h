#pragma once
#include "Object.h"
using namespace DirectX;
using namespace std;

namespace Matrix
{
	XMMATRIX view = XMMatrixTranslation(0, 0, -150);
	XMMATRIX target = XMMatrixTranslation(0, 43.265, -90);
	XMMATRIX viewer;
	XMMATRIX camera;
	float FOV = 2.0f;

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
		float translationSpeed = 0.5f;
		float zLocal = 0.0f;
		float xLocal = 0.0f;
		float yGlobal = 0.0f;
		if (GetAsyncKeyState('W'))
			zLocal += translationSpeed;
		if (GetAsyncKeyState('S'))
			zLocal -= translationSpeed;
		if (GetAsyncKeyState('A'))
			xLocal -= translationSpeed;
		if (GetAsyncKeyState('D'))
			xLocal += translationSpeed;
		if (GetAsyncKeyState(VK_SPACE))
			yGlobal += translationSpeed;
		if (GetAsyncKeyState(VK_SHIFT))
			yGlobal -= translationSpeed;

		float yRot = 0.0f;
		if (GetAsyncKeyState(VK_LEFT))
			yRot -= 0.01f;
		if (GetAsyncKeyState(VK_RIGHT))
			yRot += 0.01f;
		float xRot = 0.0f;
		if (GetAsyncKeyState(VK_UP))
			xRot -= 0.01f;
		if (GetAsyncKeyState(VK_DOWN))
			xRot += 0.01f;

		XMMATRIX translation = XMMatrixTranslation(0.0f, yGlobal, 0.0f);
		view = XMMatrixMultiply(view, translation);
		translation = XMMatrixTranslation(xLocal, 0.0f, zLocal);
		view = XMMatrixMultiply(translation, view);


		XMVECTOR temp = view.r[3];
		view.r[3] = XMMatrixIdentity().r[3];
		XMMATRIX rotation = XMMatrixRotationY(yRot);
		view = XMMatrixMultiply(view, rotation);
		view.r[3] = temp;


		rotation = XMMatrixRotationX(xRot);
		view = XMMatrixMultiply(rotation, view);


		viewer = XMMatrixMultiply(view, target);
	}
}