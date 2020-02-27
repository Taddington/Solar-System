#pragma once

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

namespace Structs
{
	struct ConstantBuffer
	{
		XMFLOAT4X4 wMatrix;
		XMFLOAT4X4 vMatrix;
		XMFLOAT4X4 pMatrix;
		XMFLOAT4 timer;
	};

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Nrm;
		XMFLOAT2 Tex;
	};

	struct Mesh
	{
		vector<Vertex> vertexList;
		vector<int> indexList;
	};
}

namespace Variables
{
	ID3D11Device* device;
	IDXGISwapChain* swapChain;
	ID3D11DeviceContext* deviceContext;
	D3D11_VIEWPORT viewPort;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11Buffer* constantBuffer;
	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilView* depthBufferView;
	ID3D11SamplerState* samplerState;
	Structs::ConstantBuffer constants;
}

