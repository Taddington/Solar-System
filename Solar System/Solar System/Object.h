#pragma once
#include "DirectXVariables.h"
using namespace DirectX;
using namespace std;

class Object
{
public:
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureResourceView;
	ID3D11Texture2D* texture2;
	ID3D11ShaderResourceView* textureResourceView2;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	Structs::Mesh mesh;

	Object();
	~Object();
	void LoadMesh(const char* MeshFileName);
	void ScaleMesh(float Scalar);
	void CreateVertexBufferAndIndexBuffer(ID3D11Device* Device);
	void CreateVertexShaderAndPixelShaderAndInputLayout(ID3D11Device* Device, const void* VertexShader, SIZE_T SizeOfVertexShader, const void* PixelShader, SIZE_T SizeOfPixelShader);
	void CreateTexture(ID3D11Device* Device, const wchar_t* Filename);
	void CreateTexture2(ID3D11Device* Device, const wchar_t* Filename);
	void RenderMesh(ID3D11DeviceContext* DeviceContext, XMMATRIX WorldPosition);
	void CreateSkyBox();
	void Release();
};

Object::Object()
{
	inputLayout = nullptr;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	texture = nullptr;
	textureResourceView = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
}

Object::~Object()
{
}

void Object::LoadMesh(const char* MeshFileName)
{
	fstream file{ MeshFileName, ios_base::in | ios_base::binary };

	assert(file.is_open());

	uint32_t player_index_count;
	file.read((char*)&player_index_count, sizeof(uint32_t));

	mesh.indexList.resize(player_index_count);

	file.read((char*)mesh.indexList.data(), sizeof(uint32_t) * player_index_count);

	uint32_t player_vertex_count;
	file.read((char*)&player_vertex_count, sizeof(uint32_t));

	mesh.vertexList.resize(player_vertex_count);

	file.read((char*)mesh.vertexList.data(), sizeof(Structs::Vertex) * player_vertex_count);

	file.close();
}

void Object::ScaleMesh(float Scalar)
{
	for (int i = 0; i < mesh.vertexList.size(); ++i)
	{
		mesh.vertexList[i].Pos.x *= Scalar;
		mesh.vertexList[i].Pos.y *= Scalar;
		mesh.vertexList[i].Pos.z *= Scalar;
	}
}

void Object::CreateVertexBufferAndIndexBuffer(ID3D11Device* Device)
{
	D3D11_BUFFER_DESC bDesc;
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&bDesc, sizeof(bDesc));
	ZeroMemory(&subData, sizeof(subData));

	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.ByteWidth = sizeof(Structs::Mesh) * (mesh.vertexList.size());
	bDesc.CPUAccessFlags = 0;
	bDesc.MiscFlags = 0;
	bDesc.StructureByteStride = 0;
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;

	subData.pSysMem = mesh.vertexList.data();
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	HRESULT hr = Device->CreateBuffer(&bDesc, &subData, &vertexBuffer);

	bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bDesc.ByteWidth = sizeof(int) * (mesh.indexList.size());
	subData.pSysMem = mesh.indexList.data();
	hr = Device->CreateBuffer(&bDesc, &subData, &indexBuffer);
}

void Object::CreateVertexShaderAndPixelShaderAndInputLayout(ID3D11Device* Device, const void* VertexShader, SIZE_T SizeOfVertexShader, const void* PixelShader, SIZE_T SizeOfPixelShader)
{
	HRESULT hr = Device->CreateVertexShader(VertexShader, SizeOfVertexShader, nullptr, &vertexShader);
	hr = Device->CreatePixelShader(PixelShader, SizeOfPixelShader, nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	hr = Device->CreateInputLayout(inputDesc, 3, VertexShader, SizeOfVertexShader, &inputLayout);
}

void Object::CreateTexture(ID3D11Device* Device, const wchar_t* Filename)
{
	HRESULT hr = CreateDDSTextureFromFile(Device, Filename, (ID3D11Resource**)&texture, &textureResourceView);
}

void Object::CreateTexture2(ID3D11Device* Device, const wchar_t* Filename)
{
	HRESULT hr = CreateDDSTextureFromFile(Device, Filename, (ID3D11Resource**)&texture2, &textureResourceView2);
}

void Object::RenderMesh(ID3D11DeviceContext* DeviceContext, XMMATRIX WorldPosition)
{
	if (texture2 && textureResourceView2)
	{
		ID3D11ShaderResourceView* ShaderResourceViews[] = { textureResourceView, textureResourceView2 };
		DeviceContext->PSSetShaderResources(0, 2, ShaderResourceViews);
	}
	else
	{
		ID3D11ShaderResourceView* ShaderResourceViews[] = { textureResourceView };
		DeviceContext->PSSetShaderResources(0, 1, ShaderResourceViews);
	}

	DeviceContext->IASetInputLayout(inputLayout);
	UINT Strides[] = { sizeof(Structs::Mesh) };
	UINT Offsets[] = { 0 };
	ID3D11Buffer* VertexBuffers[] = { vertexBuffer };
	DeviceContext->IASetVertexBuffers(0, 1, VertexBuffers, Strides, Offsets);
	DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DeviceContext->PSSetSamplers(0, 1, &Variables::samplerState);

	DeviceContext->VSSetShader(vertexShader, 0, 0);
	DeviceContext->PSSetShader(pixelShader, 0, 0);

	XMStoreFloat4x4(&Variables::constants.WorldMatrix, WorldPosition);

	D3D11_MAPPED_SUBRESOURCE gpuBuffer;
	HRESULT hr = DeviceContext->Map(Variables::constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	*((Structs::ConstantBuffer*)(gpuBuffer.pData)) = Variables::constants;
	DeviceContext->Unmap(Variables::constantBuffer, 0);

	ID3D11Buffer* constantBuffers[] = { Variables::constantBuffer };
	DeviceContext->VSSetConstantBuffers(0, 1, constantBuffers);
	DeviceContext->PSSetConstantBuffers(0, 1, constantBuffers);

	DeviceContext->DrawIndexed(mesh.indexList.size(), 0, 0);
}

void Object::CreateSkyBox()
{
	mesh.vertexList = {
		{{ 1.0f, -1.0f,  1.0f } },
		{{ 1.0f,  1.0f,  1.0f } },
		{{ 1.0f,  1.0f, -1.0f } },

		{{ 1.0f, -1.0f, -1.0f } },
		{{-1.0f, -1.0f, -1.0f } },
		{{-1.0f,  1.0f, -1.0f } },

		{{-1.0f,  1.0f,  1.0f } },
		{{-1.0f, -1.0f,  1.0f } },
		{{-1.0f, -1.0f,  1.0f } },

		{{ 1.0f, -1.0f,  1.0f } },
		{{ 1.0f, -1.0f, -1.0f } },
		{{-1.0f, -1.0f, -1.0f } },

		{{ 1.0f,  1.0f, -1.0f } },
		{{ 1.0f,  1.0f,  1.0f } },
		{{-1.0f,  1.0f,  1.0f } },

		{{-1.0f,  1.0f, -1.0f } },
		{{-1.0f,  1.0f,  1.0f } },
		{{ 1.0f,  1.0f,  1.0f } },

		{{ 1.0f, -1.0f,  1.0f } },
		{{-1.0f, -1.0f,  1.0f } },
		{{ 1.0f, -1.0f, -1.0f } },

		{{ 1.0f,  1.0f, -1.0f } },
		{{-1.0f,  1.0f, -1.0f } },
		{{-1.0f, -1.0f, -1.0f } }

	};

	mesh.indexList = {
		0,  1,  2,
		0,  2,  3,
		4,  5,  6,
		4,  6,  7,
		8,  9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};
}

void Object::Release()
{
	inputLayout->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
	texture->Release();
	textureResourceView->Release();
	if (texture2)
		texture2->Release();
	if (textureResourceView2)
		textureResourceView2->Release();
	vertexShader->Release();
	pixelShader->Release();
}