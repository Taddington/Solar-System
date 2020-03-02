// Solar System.cpp : Defines the entry point for the application.
//

#include "Matrix.h"

#include "SkyboxVertexShader.csh"
#include "SkyboxPixelShader.csh"
#include "ModelVertexShader.csh"
#include "ModelPixelShader.csh"
#include "SunPixelShader.csh"
#include "EarthPixelShader.csh"

Object Skybox;
Object Sun;
Object Mercury;
Object Venus;
Object Earth;
Object Moon;
Object Mars;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND window;									// Store the hWnd

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SOLARSYSTEM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOLARSYSTEM));

	MSG msg;

	// Main message loop:
	while (true)
	{
		PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
			break;

		RECT rect;
		GetClientRect(window, &rect);
		float aspectRatio = (rect.right - rect.left) / float(rect.bottom - rect.top);


		Matrix::GetInputForCamera();
		XMStoreFloat4x4(&Variables::constants.ViewMatrix, Matrix::camera);

		XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(3.14f / Matrix::FOV, aspectRatio, 0.1f, 5000.0f);
		XMStoreFloat4x4(&Variables::constants.ProjectionMatrix, projectionMatrix);

		static float timer = 0; timer += 0.0025f;
		XMVECTOR time = XMVectorSet(timer, 0.0f, 0.0f, 0.0f);
		XMStoreFloat4(&Variables::constants.Time, time);


		Variables::deviceContext->OMSetRenderTargets(1, &Variables::renderTargetView, Variables::depthBufferView);

		Variables::deviceContext->RSSetViewports(1, &Variables::viewPort);

		float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Variables::deviceContext->ClearRenderTargetView(Variables::renderTargetView, color);



		XMMATRIX worldMatrix = XMMatrixTranslationFromVector(XMMatrixInverse(0, Matrix::camera).r[3]);
		Skybox.RenderMesh(Variables::deviceContext, worldMatrix);

		Variables::deviceContext->ClearDepthStencilView(Variables::depthBufferView, D3D11_CLEAR_DEPTH, 1, 0);

		Matrix::FOV = 2.0f;
		if (!GetAsyncKeyState('0'))
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::view);
		static float sunRotation = 0.0f; sunRotation += -0.0123f;
		worldMatrix = XMMatrixIdentity();
		worldMatrix = XMMatrixMultiply(XMMatrixRotationY(sunRotation), worldMatrix);
		if (GetAsyncKeyState('0'))
		{
			Matrix::FOV = 2.0f;
			Matrix::target = XMMatrixMultiply(XMMatrixRotationY(-sunRotation), worldMatrix);
			Matrix::target = XMMatrixMultiply(XMMatrixTranslation(0, 43.265, -86.53f), Matrix::target);
			Matrix::view = XMMatrixTranslation(0, 43.265f, -90.0f);
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::look_at(Matrix::viewer.r[3], Matrix::target.r[3], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		}
		Sun.RenderMesh(Variables::deviceContext, worldMatrix);

		static float mercuryRotation = 0.0f; mercuryRotation += -0.0062f;
		static float mercuryOrbit = 0.0f; mercuryOrbit += -0.004f;
		worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(-0.0349066f), XMMatrixTranslation(122.133f, 0.0f, 0.0f));
		worldMatrix = XMMatrixMultiply(XMMatrixRotationY(mercuryRotation), worldMatrix);
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(mercuryOrbit));
		if (GetAsyncKeyState('1'))
		{
			Matrix::FOV = 50.0f;
			Matrix::target = XMMatrixMultiply(XMMatrixRotationY(-mercuryRotation), worldMatrix);
			Matrix::view = XMMatrixTranslation(0, 43.265, -90);
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::look_at(Matrix::viewer.r[3], Matrix::target.r[3], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		}
		Mercury.RenderMesh(Variables::deviceContext, worldMatrix);

		static float venusRotation = 0.0f; venusRotation += 0.0021f;
		static float venusOrbit = 0.0f; venusOrbit += -0.0016f;
		worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(-0.0523599f), XMMatrixTranslation(153.373f, 0.0f, 0.0f));
		worldMatrix = XMMatrixMultiply(XMMatrixRotationY(venusRotation), worldMatrix);
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(venusOrbit));
		if (GetAsyncKeyState('2'))
		{
			Matrix::FOV = 50.0f;
			Matrix::target = XMMatrixMultiply(XMMatrixRotationY(-venusRotation), worldMatrix);
			Matrix::view = XMMatrixTranslation(0, 43.265, -90);
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::look_at(Matrix::viewer.r[3], Matrix::target.r[3], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		}
		Venus.RenderMesh(Variables::deviceContext, worldMatrix);

		static float earthRotation = 0.0f; earthRotation += -0.36f;
		static float earthOrbit = 0.0f; earthOrbit += -0.00098f;
		worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(-0.408407f), XMMatrixTranslation(178.647f, 0.0f, 0.0f));
		worldMatrix = XMMatrixMultiply(XMMatrixRotationY(earthRotation), worldMatrix);
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(earthOrbit));
		if (GetAsyncKeyState('3'))
		{
			Matrix::FOV = 50.0f;
			Matrix::target = XMMatrixMultiply(XMMatrixRotationY(-earthRotation), worldMatrix);
			Matrix::view = XMMatrixTranslation(0, 43.265, -90);
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::look_at(Matrix::viewer.r[3], Matrix::target.r[3], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		}
		Earth.RenderMesh(Variables::deviceContext, worldMatrix);

		static float moonOrbitAndRotation = 0.0f; moonOrbitAndRotation += -0.013f;
		worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(-0.408407f), XMMatrixTranslation(178.647f, 0.0f, 0.0f));
		worldMatrix = XMMatrixMultiply(XMMatrixRotationY(moonOrbitAndRotation), worldMatrix);
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(earthOrbit));
		XMMATRIX tempMat = XMMatrixTranslation(1.034f, 0.0f, 0.0f);
		XMMATRIX tempRot = XMMatrixRotationY(moonOrbitAndRotation);
		tempMat = XMMatrixMultiply(tempMat, tempRot);
		worldMatrix = XMMatrixMultiply(tempMat, worldMatrix);
		Moon.RenderMesh(Variables::deviceContext, worldMatrix);

		static float marsRotation = 0.0f; marsRotation += -0.36f;
		static float marsOrbit = 0.0f; marsOrbit += -0.00052f;
		worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(-0.436332f), XMMatrixTranslation(227.354f, 0.0f, 0.0f));
		worldMatrix = XMMatrixMultiply(XMMatrixRotationY(marsRotation), worldMatrix);
		worldMatrix = XMMatrixMultiply(worldMatrix, XMMatrixRotationY(marsOrbit));
		if (GetAsyncKeyState('4'))
		{
			Matrix::FOV = 50.0f;
			Matrix::target = XMMatrixMultiply(XMMatrixRotationY(-marsRotation), worldMatrix);
			Matrix::view = XMMatrixTranslation(0, 43.265, -90);
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::look_at(Matrix::viewer.r[3], Matrix::target.r[3], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		}
		Mars.RenderMesh(Variables::deviceContext, worldMatrix);

		Variables::swapChain->Present(0, 0);
	}
	Mars.Release();
	Moon.Release();
	Earth.Release();
	Venus.Release();
	Mercury.Release();
	Sun.Release();
	Skybox.Release();
	Variables::depthBufferView->Release();
	Variables::depthBuffer->Release();
	Variables::constantBuffer->Release();
	Variables::samplerState->Release();
	Variables::renderTargetView->Release();
	Variables::deviceContext->Release();
	Variables::swapChain->Release();
	Variables::device->Release();

	return (int)msg.wParam;
}



//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	RECT myWinR;
	GetClientRect(hWnd, &myWinR);
	window = hWnd;

	Setup::SetupDirectX(Variables::device, Variables::swapChain, Variables::deviceContext, Variables::viewPort, Variables::renderTargetView, hWnd, myWinR);

	Setup::CreateSamplerState(Variables::samplerState, Variables::device);

	Setup::CreateConstantBuffer(Variables::constantBuffer, Variables::device, sizeof(Structs::ConstantBuffer));

	Setup::CreateDepthBuffer(Variables::depthBuffer, Variables::depthBufferView, Variables::device, myWinR);

	Skybox.CreateSkyBox();
	Skybox.CreateVertexBufferAndIndexBuffer(Variables::device);
	Skybox.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, SkyboxVertexShader, sizeof(SkyboxVertexShader), SkyboxPixelShader, sizeof(SkyboxPixelShader));
	Skybox.CreateTexture(Variables::device, L"Assets/Skybox_Space.dds");

	Sun.LoadMesh("Assets/planet.mesh");
	Sun.ScaleMesh(86.53f);
	Sun.CreateVertexBufferAndIndexBuffer(Variables::device);
	Sun.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), SunPixelShader, sizeof(SunPixelShader));
	Sun.CreateTexture(Variables::device, L"Assets/SunTexture.dds");

	Mercury.LoadMesh("Assets/planet.mesh");
	Mercury.ScaleMesh(0.3f);
	Mercury.CreateVertexBufferAndIndexBuffer(Variables::device);
	Mercury.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Mercury.CreateTexture(Variables::device, L"Assets/MercuryTexture.dds");

	Venus.LoadMesh("Assets/planet.mesh");
	Venus.ScaleMesh(0.75f);
	Venus.CreateVertexBufferAndIndexBuffer(Variables::device);
	Venus.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Venus.CreateTexture(Variables::device, L"Assets/VenusTexture.dds");

	Earth.LoadMesh("Assets/planet.mesh");
	Earth.ScaleMesh(0.79f);
	Earth.CreateVertexBufferAndIndexBuffer(Variables::device);
	Earth.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), EarthPixelShader, sizeof(EarthPixelShader));
	Earth.CreateTexture(Variables::device, L"Assets/DayEarthTexture.dds");
	Earth.CreateTexture2(Variables::device, L"Assets/NightEarthTexture.dds");

	Moon.LoadMesh("Assets/planet.mesh");
	Moon.ScaleMesh(0.21f);
	Moon.CreateVertexBufferAndIndexBuffer(Variables::device);
	Moon.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Moon.CreateTexture(Variables::device, L"Assets/MoonTexture.dds");

	Mars.LoadMesh("Assets/planet.mesh");
	Mars.ScaleMesh(0.42f);
	Mars.CreateVertexBufferAndIndexBuffer(Variables::device);
	Mars.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Mars.CreateTexture(Variables::device, L"Assets/MarsTexture.dds");

	return TRUE;
}





//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOLARSYSTEM));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SOLARSYSTEM);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}