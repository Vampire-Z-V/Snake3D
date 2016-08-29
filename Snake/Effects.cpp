#include "Effects.h"

#pragma region LoadEffect
LoadEffect::LoadEffect(ID3D11Device * device, const std::wstring & filename)
:	m_pFX(nullptr)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios::beg);
	std::vector<char> compileShader(size);

	fin.read(&compileShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compileShader[0], size, 0, device, &m_pFX));
}

LoadEffect::~LoadEffect()
{
	ReleaseCOM(m_pFX);
}
#pragma endregion

#pragma region BasicEffect
BasicEffect::BasicEffect(ID3D11Device * device, const std::wstring & filename)
:	LoadEffect(device, filename)
{
	m_pLight3TexTech			= m_pFX->GetTechniqueByName("Light3Tex");
	m_pLight3TexAlphaClipTech	= m_pFX->GetTechniqueByName("Light3TexAlphaClip");

	m_pWorldViewProj     = m_pFX->GetVariableByName("g_WorldViewProj")->AsMatrix();
	m_pWorld             = m_pFX->GetVariableByName("g_World")->AsMatrix();
	m_pWorldInvTranspose = m_pFX->GetVariableByName("g_WorldInvTranspose")->AsMatrix();
	m_pTexTransform      = m_pFX->GetVariableByName("g_TexTransform")->AsMatrix();
	m_pEyePosW           = m_pFX->GetVariableByName("g_EyePosW")->AsVector();
	m_pDirLights         = m_pFX->GetVariableByName("g_DirLights");
	m_pMaterial          = m_pFX->GetVariableByName("g_Material");
	m_pDiffuseMap        = m_pFX->GetVariableByName("g_DiffuseMap")->AsShaderResource();
}
#pragma endregion

#pragma region Effects
BasicEffect* Effects::pBasicFX = nullptr;

void Effects::InitAll(ID3D11Device * device)
{
	pBasicFX = new BasicEffect(device, L"FX/Effect.fxo");
}

void Effects::DestroyAll()
{
	SafeDelete(pBasicFX);
}
#pragma endregion