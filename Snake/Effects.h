#pragma once
#include "D3DUtility.h"

class LoadEffect
{
public:
	LoadEffect(ID3D11Device *device,const std::wstring &filename);
	virtual ~LoadEffect();

protected:
	ID3DX11Effect* m_pFX;
};

class BasicEffect :	
	public LoadEffect
{
public:
	BasicEffect(ID3D11Device *device, const std::wstring &filename);

	// Setter
	void SetWorldViewProj		(CXMMATRIX M)						{ m_pWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld				(CXMMATRIX M)						{ m_pWorld->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose	(CXMMATRIX M)						{ m_pWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform		(CXMMATRIX M)						{ m_pTexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW				(const XMFLOAT3 &v)					{ m_pEyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights			(const DirectionalLight *lights)	{ m_pDirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial			(const Material &mat)				{ m_pMaterial->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap			(ID3D11ShaderResourceView *tex)		{ m_pDiffuseMap->SetResource(tex); }

	// Getter
	const ID3DX11EffectTechnique* GetLight3TexTech()const			{ return m_pLight3TexTech; }
	const ID3DX11EffectTechnique* GetLight3TexAlphaClipTech()const	{ return m_pLight3TexAlphaClipTech; }

private:
	ID3DX11EffectTechnique*	m_pLight3TexTech;
	ID3DX11EffectTechnique*	m_pLight3TexAlphaClipTech;

	ID3DX11EffectMatrixVariable* m_pWorldViewProj;
	ID3DX11EffectMatrixVariable* m_pWorld;
	ID3DX11EffectMatrixVariable* m_pWorldInvTranspose;
	ID3DX11EffectMatrixVariable* m_pTexTransform;
								 
	ID3DX11EffectVectorVariable* m_pEyePosW;

	ID3DX11EffectVariable* m_pDirLights;
	ID3DX11EffectVariable* m_pMaterial;

	ID3DX11EffectShaderResourceVariable* m_pDiffuseMap;
};

class Effects
{
public:
	static void InitAll(ID3D11Device *device);
	static void DestroyAll();

	static BasicEffect* pBasicFX;
};
