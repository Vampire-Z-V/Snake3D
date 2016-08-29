#pragma once
#include "D3DUtility.h"

class InputLayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC BasicVertex[3];
};

class InputLayouts
{
public:
	static ID3D11InputLayout* BasicVertex;

	static void InitAll(ID3D11Device *device);
	static void DestroyAll();
};
