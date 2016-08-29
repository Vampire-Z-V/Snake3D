#include "InputLayouts.h"
#include "Effects.h"

#pragma region InputLayoutDesc

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::BasicVertex[3] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

#pragma endregion

#pragma region InputLayouts

ID3D11InputLayout* InputLayouts::BasicVertex = 0;

void InputLayouts::InitAll(ID3D11Device* device)
{
	D3DX11_PASS_DESC passDesc;

	//
	// BasicVertex
	//

	const_cast<ID3DX11EffectTechnique*> (Effects::pBasicFX->GetLight3TexTech())->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(InputLayoutDesc::BasicVertex, 3, 
								 passDesc.pIAInputSignature,
								 passDesc.IAInputSignatureSize, 
								 &BasicVertex));
}

void InputLayouts::DestroyAll()
{
	ReleaseCOM(BasicVertex);
}

#pragma endregion
