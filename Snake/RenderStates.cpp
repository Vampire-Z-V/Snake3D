#include "RenderStates.h"

ID3D11RasterizerState*   RenderStates::pWireframeRS			= nullptr;
ID3D11RasterizerState*   RenderStates::pNoCullRS			= nullptr;

ID3D11DepthStencilState* RenderStates::pEqualsDSS			= nullptr;

ID3D11BlendState*        RenderStates::pAlphaToCoverageBS	= nullptr;
ID3D11BlendState*        RenderStates::pTransparentBS		= nullptr;

void RenderStates::InitAll(ID3D11Device* device)
{
	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, &pWireframeRS));

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&noCullDesc, &pNoCullRS));

	//
	// EqualsDSS
	//
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	equalsDesc.DepthFunc = D3D11_COMPARISON_EQUAL;

	HR(device->CreateDepthStencilState(&equalsDesc, &pEqualsDSS));

	//
	// AlphaToCoverageBS
	//
	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&alphaToCoverageDesc, &pAlphaToCoverageBS));

	//
	// TransparentBS
	//
	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&transparentDesc, &pTransparentBS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(pWireframeRS);
	ReleaseCOM(pNoCullRS);
	ReleaseCOM(pEqualsDSS);
	ReleaseCOM(pAlphaToCoverageBS);
	ReleaseCOM(pTransparentBS);
}