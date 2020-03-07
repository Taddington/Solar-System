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
Object Jupiter;
Object Saturn;
Object Uranus;
Object Neptune;
Object Pluto;
Object SaturnRing;
Object UranusRing;

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



		Skybox.worldMatrix = XMMatrixTranslationFromVector(XMMatrixInverse(0, Matrix::camera).r[3]);
		Skybox.RenderMesh(Variables::deviceContext);

		Variables::deviceContext->ClearDepthStencilView(Variables::depthBufferView, D3D11_CLEAR_DEPTH, 1, 0);

		Matrix::FOV = 2.0f;
		if (!GetAsyncKeyState('0'))
			Matrix::camera = XMMatrixInverse(nullptr, Matrix::view);

		static float sunRotation = 0.0f; sunRotation += -0.0123f;
		Matrix::CreateSunRotation(Sun, sunRotation);
		Sun.RenderMesh(Variables::deviceContext);

		static float mercuryRotation = 0.0f; mercuryRotation += -0.0062f;
		static float mercuryOrbit = 0.0f; mercuryOrbit += -0.004f;
		Matrix::CreatePlanetOrbit(Mercury, mercuryRotation, mercuryOrbit, -0.0349066f, 122.133f, '1', 50.0f);
		Mercury.RenderMesh(Variables::deviceContext);

		static float venusRotation = 0.0f; venusRotation += 0.0021f;
		static float venusOrbit = 0.0f; venusOrbit += -0.0016f;
		Matrix::CreatePlanetOrbit(Venus, venusRotation, venusOrbit, -0.0523599f, 153.373f, '2', 50.0f);
		Venus.RenderMesh(Variables::deviceContext);

		static float earthRotation = 0.0f; earthRotation += -0.36f;
		static float earthOrbit = 0.0f; earthOrbit += -0.00098f;
		Matrix::CreatePlanetOrbit(Earth, earthRotation, earthOrbit, -0.408407f, 178.647f, '3', 50.0f);
		Earth.RenderMesh(Variables::deviceContext);

		static float moonOrbitAndRotation = 0.0f; moonOrbitAndRotation += -0.013f;
		Matrix::CreateMoonOrbit(Moon, moonOrbitAndRotation, earthOrbit, -0.408407f, 178.647f, 1.034f);
		Moon.RenderMesh(Variables::deviceContext);

		static float marsRotation = 0.0f; marsRotation += -0.36f;
		static float marsOrbit = 0.0f; marsOrbit += -0.00052f;
		Matrix::CreatePlanetOrbit(Mars, marsRotation, marsOrbit, -0.436332f, 227.354f, '4', 50.0f);
		Mars.RenderMesh(Variables::deviceContext);

		static float jupiterRotation = 0.0f; jupiterRotation += -0.8571f;
		static float jupiterOrbit = 0.0f; jupiterOrbit += -0.000083f;
		Matrix::CreatePlanetOrbit(Jupiter, jupiterRotation, jupiterOrbit, -0.0523599f, 570.4f, '5', 5.0f);
		Jupiter.RenderMesh(Variables::deviceContext);

		static float saturnRotation = 0.0f; saturnRotation += -0.7826f;
		static float saturnOrbit = 0.0f; saturnOrbit += -0.000034f;
		Matrix::CreatePlanetOrbit(Saturn, saturnRotation, saturnOrbit, -0.46652651f, 1018.685f, '6', 5.0f);
		Saturn.RenderMesh(Variables::deviceContext);

		Matrix::CreateRing(SaturnRing, saturnOrbit, 1018.685f, -0.46652651f);
		SaturnRing.worldMatrix = XMMatrixMultiply(XMMatrixRotationX(-4.6f), SaturnRing.worldMatrix);
		SaturnRing.RenderMesh(Variables::deviceContext);

		static float uranusRotation = 0.0f; uranusRotation += 0.5070f;
		static float uranusOrbit = 0.0f; uranusOrbit += -0.000011f;
		Matrix::CreatePlanetOrbit(Uranus, uranusRotation, uranusOrbit, -1.7064084f, 1983.055f, '7', 10.0f);
		Uranus.RenderMesh(Variables::deviceContext);

		Matrix::CreateRing(UranusRing, uranusOrbit, 1983.055f, -1.7064084f);
		UranusRing.RenderMesh(Variables::deviceContext);

		static float neptuneRotation = 0.0f; neptuneRotation += -0.5373f;
		static float neptuneOrbit = 0.0f; neptuneOrbit += -0.0000059f;
		Matrix::CreatePlanetOrbit(Neptune, neptuneRotation, neptuneOrbit, -0.488692f, 2868.874f, '8', 10.0f);
		Neptune.RenderMesh(Variables::deviceContext);

		static float plutoRotation = 0.0f; plutoRotation += -0.5373f;
		static float plutoOrbit = 0.0f; plutoOrbit += -0.0000059f;
		Matrix::CreatePlanetOrbit(Pluto, plutoRotation, plutoOrbit, -0.994838f, 3246.144f, '9', 100.0f);
		Pluto.RenderMesh(Variables::deviceContext);

		Variables::swapChain->Present(0, 0);
	}
	UranusRing.Release();
	SaturnRing.Release();
	Pluto.Release();
	Neptune.Release();
	Uranus.Release();
	Saturn.Release();
	Jupiter.Release();
	Mars.Release();
	Moon.Release();
	Earth.Release();
	Venus.Release();
	Mercury.Release();
	Sun.Release();
	Skybox.Release();
	Variables::ReleaseVariables();

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

	Jupiter.LoadMesh("Assets/planet.mesh");
	Jupiter.ScaleMesh(8.68f);
	Jupiter.CreateVertexBufferAndIndexBuffer(Variables::device);
	Jupiter.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Jupiter.CreateTexture(Variables::device, L"Assets/JupiterTexture.dds");

	Saturn.LoadMesh("Assets/planet.mesh");
	Saturn.ScaleMesh(7.23f);
	Saturn.CreateVertexBufferAndIndexBuffer(Variables::device);
	Saturn.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Saturn.CreateTexture(Variables::device, L"Assets/SaturnTexture.dds");

	Uranus.LoadMesh("Assets/planet.mesh");
	Uranus.ScaleMesh(3.15f);
	Uranus.CreateVertexBufferAndIndexBuffer(Variables::device);
	Uranus.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Uranus.CreateTexture(Variables::device, L"Assets/UranusTexture.dds");

	Neptune.LoadMesh("Assets/planet.mesh");
	Neptune.ScaleMesh(3.05f);
	Neptune.CreateVertexBufferAndIndexBuffer(Variables::device);
	Neptune.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Neptune.CreateTexture(Variables::device, L"Assets/NeptuneTexture.dds");

	Pluto.LoadMesh("Assets/planet.mesh");
	Pluto.ScaleMesh(0.14f);
	Pluto.CreateVertexBufferAndIndexBuffer(Variables::device);
	Pluto.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Pluto.CreateTexture(Variables::device, L"Assets/PlutoTexture.dds");

	SaturnRing.LoadMesh("Assets/saturnring.mesh");
	SaturnRing.ScaleMesh(1.25f);
	SaturnRing.CreateVertexBufferAndIndexBuffer(Variables::device);
	SaturnRing.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	SaturnRing.CreateTexture(Variables::device, L"Assets/SaturnRingTexture.dds");

	UranusRing.LoadMesh("Assets/saturnring.mesh");
	UranusRing.ScaleMesh(0.5f);
	UranusRing.CreateVertexBufferAndIndexBuffer(Variables::device);
	UranusRing.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	UranusRing.CreateTexture(Variables::device, L"Assets/SaturnRingTexture.dds");

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