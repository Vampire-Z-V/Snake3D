#pragma once
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <string>

namespace BaseWindow
{
	extern HINSTANCE	g_hInstance;
	extern HWND			g_hMainWnd;
	 
	extern std::wstring	g_wsMainWndCaption;
	extern int			g_nClientWidth;
	extern int			g_nClientHeight;

	extern bool			g_bMaximized;
	extern bool			g_bMinimized;
	extern bool			g_bResizing;

	HRESULT Init(HINSTANCE hInstance);
	float GetAspectRatio();
}

namespace D3DApplication
{
	extern UINT						g_4xMsaaQuality;
	extern bool						g_Enable4xMsaa;

	extern D3D_DRIVER_TYPE			g_D3DDriverType;

	extern ID3D11Device*			g_pD3DDevice;
	extern ID3D11DeviceContext*		g_pD3DImmediateContext;
	extern IDXGISwapChain*			g_pSwapChain;
	extern ID3D11Texture2D*			g_pDepthStencilBuffer;
	extern ID3D11RenderTargetView*	g_pRenderTargetView;
	extern ID3D11DepthStencilView*	g_pDepthStencilView;

	extern D3D11_VIEWPORT			g_ScreenViewport;

	HRESULT Init();
	void BaseUpdate();
	void ClearDevice();
}

namespace D2D1_And_DirectWrite
{
	extern ID2D1Factory*			g_pD2DFactory;
	extern ID2D1RenderTarget*		g_pD2DRenderTarget;
	extern ID2D1SolidColorBrush*	g_pD2DSolidBrush_Headline;
	extern ID2D1SolidColorBrush*	g_pD2DSolidBrush_Option_Normal;
	extern ID2D1SolidColorBrush*	g_pD2DSolidBrush_Option_Chosen;

	extern IDWriteFactory*			g_pDWFactory;
	extern IDWriteTextLayout*		g_pDWTextLayout;
	extern IDWriteTextFormat*		g_pDWTextFormat_Headline;
	extern IDWriteTextFormat*		g_pDWTextFormat_Option_Normal;
	extern IDWriteTextFormat*		g_pDWTextFormat_Option_Chosen;

	extern D2D1_COLOR_F				g_FontColor_Headline;
	extern D2D1_COLOR_F				g_FontColor_Option_Normal;
	extern D2D1_COLOR_F				g_FontColor_Option_Chosen;

	HRESULT Init();
	void BaseUpdate();
	void ClearDevice();
}