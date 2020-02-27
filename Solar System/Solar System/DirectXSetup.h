#pragma once
#include "framework.h"
#include "Solar System.h"
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "DDSTextureLoader.h"
#include <DirectXMath.h>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <fstream>
using namespace DirectX;
using namespace std;

namespace Setup
{
	void SetupDirectX(ID3D11Device* &Device, IDXGISwapChain* &SwapChain, ID3D11DeviceContext* &DeviceContext, D3D11_VIEWPORT &ViewPort, ID3D11RenderTargetView* &RenderTargetView, HWND hWnd, RECT myWinR)
	{
		D3D_FEATURE_LEVEL dx11 = D3D_FEATURE_LEVEL_11_0;
		DXGI_SWAP_CHAIN_DESC swap;
		ZeroMemory(&swap, sizeof(DXGI_SWAP_CHAIN_DESC));
		swap.BufferCount = 1;
		swap.OutputWindow = hWnd;
		swap.Windowed = true;
		swap.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swap.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap.BufferDesc.Width = myWinR.right - myWinR.left;
		swap.BufferDesc.Height = myWinR.bottom - myWinR.top;
		swap.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap.SampleDesc.Count = 1;

		HRESULT hr;
		hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &dx11, 1, D3D11_SDK_VERSION, &swap, &SwapChain, &Device, NULL, &DeviceContext);

		ID3D11Resource* backbuffer;
		hr = SwapChain->GetBuffer(0, __uuidof(backbuffer), (void**)&backbuffer);
		hr = Device->CreateRenderTargetView(backbuffer, NULL, &RenderTargetView);

		backbuffer->Release();

		ViewPort.Width = swap.BufferDesc.Width;
		ViewPort.Height = swap.BufferDesc.Height;
		ViewPort.TopLeftX = ViewPort.TopLeftY = 0;
		ViewPort.MinDepth = 0;
		ViewPort.MaxDepth = 1;
	}

	void CreateSamplerState(ID3D11SamplerState* &SamplerState, ID3D11Device* Device)
	{
		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		HRESULT hr = Device->CreateSamplerState(&sampDesc, &SamplerState);
	}

	void CreateConstantBuffer(ID3D11Buffer* &ConstantBuffer, ID3D11Device* Device, int SizeOfConstantBuffer)
	{
		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(bDesc));

		bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bDesc.ByteWidth = SizeOfConstantBuffer;
		bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;
		bDesc.Usage = D3D11_USAGE_DYNAMIC;

		HRESULT hr = Device->CreateBuffer(&bDesc, nullptr, &ConstantBuffer);
	}

	void CreateDepthBuffer(ID3D11Texture2D* &DepthBuffer, ID3D11DepthStencilView* &DepthBufferView, ID3D11Device* Device, RECT myWinR)
	{
		D3D11_TEXTURE2D_DESC zDesc;
		ZeroMemory(&zDesc, sizeof(zDesc));
		zDesc.ArraySize = 1;
		zDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		zDesc.Width = myWinR.right - myWinR.left;
		zDesc.Height = myWinR.bottom - myWinR.top;
		zDesc.Format = DXGI_FORMAT_D32_FLOAT;
		zDesc.Usage = D3D11_USAGE_DEFAULT;
		zDesc.MipLevels = 1;
		zDesc.SampleDesc.Count = 1;

		HRESULT hr = Device->CreateTexture2D(&zDesc, nullptr, &DepthBuffer);

		hr = Device->CreateDepthStencilView(DepthBuffer, nullptr, &DepthBufferView);
	}
}