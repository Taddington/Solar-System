#pragma once

#include "DirectXSetup.h"
using namespace DirectX;
using namespace std;

namespace Structs
{
	struct ConstantBuffer
	{
		XMFLOAT4X4 WorldMatrix;
		XMFLOAT4X4 ViewMatrix;
		XMFLOAT4X4 ProjectionMatrix;
		XMFLOAT4 Time;
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

