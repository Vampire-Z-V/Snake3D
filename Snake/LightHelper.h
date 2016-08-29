#pragma once
#include <Windows.h>
#include <DirectXMath.h>
using namespace DirectX;

struct DirectionalLight
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
	float Pad;

	DirectionalLight()
	{
		ZeroMemory(this, sizeof(this));
	}
};

struct Material
{
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT4 Reflect;

	Material()
	{
		ZeroMemory(this, sizeof(this));
	}
};