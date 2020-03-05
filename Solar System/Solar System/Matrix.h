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

	void CreatePlanetOrbit(Object& planet, float rotation, float orbit, float angle, float distance, char zoom , float FOV)
	{
		planet.worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(angle), XMMatrixTranslation(distance, 0.0f, 0.0f));
		planet.worldMatrix = XMMatrixMultiply(XMMatrixRotationY(rotation), planet.worldMatrix);
		planet.worldMatrix = XMMatrixMultiply(planet.worldMatrix, XMMatrixRotationY(orbit));
		if (GetAsyncKeyState(zoom))
		{
			Matrix::FOV = FOV;
			Matrix::target = XMMatrixMultiply(XMMatrixRotationY(-rotation), planet.worldMatrix);
			Matrix::view = XMMatrixTranslation(0, 15, -30);
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::look_at(Matrix::viewer.r[3], Matrix::target.r[3], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		}
	}

	void CreateMoonOrbit(Object& moon, float rotationAndOrbit, float planetOrbit, float planetAngle, float planetDistance, float distanceFromPlanet)
	{
		moon.worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(planetAngle), XMMatrixTranslation(planetDistance, 0.0f, 0.0f));
		moon.worldMatrix = XMMatrixMultiply(XMMatrixRotationY(rotationAndOrbit), moon.worldMatrix);
		moon.worldMatrix = XMMatrixMultiply(moon.worldMatrix, XMMatrixRotationY(planetOrbit));
		XMMATRIX tempMat = XMMatrixTranslation(distanceFromPlanet, 0.0f, 0.0f);
		XMMATRIX tempRot = XMMatrixRotationY(rotationAndOrbit);
		tempMat = XMMatrixMultiply(tempMat, tempRot);
		moon.worldMatrix = XMMatrixMultiply(tempMat, moon.worldMatrix);
	}

	void CreateSunRotation(Object& sun, float rotation)
	{
		sun.worldMatrix = XMMatrixIdentity();
		sun.worldMatrix = XMMatrixMultiply(XMMatrixRotationY(rotation), sun.worldMatrix);
		if (GetAsyncKeyState('0'))
		{
			Matrix::FOV = 2.0f;
			Matrix::target = XMMatrixMultiply(XMMatrixRotationY(-rotation), sun.worldMatrix);
			Matrix::target = XMMatrixMultiply(XMMatrixTranslation(0, 43.265, -86.53f), Matrix::target);
			Matrix::view = XMMatrixTranslation(0, 43.265f, -90.0f);
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::look_at(Matrix::viewer.r[3], Matrix::target.r[3], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		}
	}
}