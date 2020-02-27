// Solar System.cpp : Defines the entry point for the application.
//

#include "Matrix.h"

#include "SkyboxVertexShader.csh"
#include "SkyboxPixelShader.csh"
#include "ModelVertexShader.csh"
#include "ModelPixelShader.csh"

Object Skybox;
Object Spaceship;



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
		XMMATRIX camera = XMMatrixInverse(nullptr, Matrix::look_at(Matrix::viewer.r[3], Matrix::target.r[3], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
		XMStoreFloat4x4(&Variables::constants.ViewMatrix, camera);

		XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(3.14f / 4, aspectRatio, 0.1f, 1000.0f);
		XMStoreFloat4x4(&Variables::constants.ProjectionMatrix, projectionMatrix);

		static float timer = 0; timer += 0.0025f;
		XMVECTOR time = XMVectorSet(timer, 0.0f, 0.0f, 0.0f);
		XMStoreFloat4(&Variables::constants.Time, time);


		Variables::deviceContext->OMSetRenderTargets(1, &Variables::renderTargetView, Variables::depthBufferView);

		Variables::deviceContext->RSSetViewports(1, &Variables::viewPort);

		float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		Variables::deviceContext->ClearRenderTargetView(Variables::renderTargetView, color);



		XMMATRIX worldMatrix = XMMatrixTranslationFromVector(XMMatrixInverse(0, camera).r[3]);
		Skybox.RenderMesh(Variables::deviceContext, worldMatrix);

		Variables::deviceContext->ClearDepthStencilView(Variables::depthBufferView, D3D11_CLEAR_DEPTH, 1, 0);



		worldMatrix = Matrix::target;
		Spaceship.RenderMesh(Variables::deviceContext, worldMatrix);

		Variables::swapChain->Present(0, 0);
	}
	Spaceship.Release();
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

	Spaceship.LoadMesh("Assets/Spaceship.mesh");
	Spaceship.ScaleMesh(0.5f);
	Spaceship.CreateVertexBufferAndIndexBuffer(Variables::device);
	Spaceship.CreateVertexShaderAndPixelShaderAndInputLayout(Variables::device, ModelVertexShader, sizeof(ModelVertexShader), ModelPixelShader, sizeof(ModelPixelShader));
	Spaceship.CreateTexture(Variables::device, L"Assets/Spaceship.dds");

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