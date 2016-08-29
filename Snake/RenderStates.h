#pragma once
#include "D3DUtility.h"

class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11RasterizerState* pWireframeRS;
	static ID3D11RasterizerState* pNoCullRS;

	static ID3D11DepthStencilState* pEqualsDSS;

	static ID3D11BlendState* pAlphaToCoverageBS;
	static ID3D11BlendState* pTransparentBS;
};