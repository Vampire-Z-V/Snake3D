#include "_BaseWindow.h"
#include "resource.h"
#include "D3DUtility.h"

extern LRESULT CALLBACK MainWndProc(HWND hwnd, UINT g_sg, WPARAM wParam, LPARAM lParam);

namespace BaseWindow
{
	HINSTANCE		g_hInstance	= nullptr;
	HWND			g_hMainWnd	= nullptr;

	std::wstring	g_wsMainWndCaption	= L"Snake V0.1";
	int				g_nClientWidth	= 1280;
	int				g_nClientHeight	= 720;

	bool			g_bMaximized = false;
	bool			g_bMinimized = false;
	bool			g_bResizing  = false;
}

namespace D3DApplication
{
	UINT					g_4xMsaaQuality = 0;
	bool					g_Enable4xMsaa	= true;

	D3D_DRIVER_TYPE			g_D3DDriverType = D3D_DRIVER_TYPE_HARDWARE;
	
	ID3D11Device*			g_pD3DDevice = nullptr;
	ID3D11DeviceContext*	g_pD3DImmediateContext = nullptr;
	IDXGISwapChain*			g_pSwapChain = nullptr;
	ID3D11Texture2D*		g_pDepthStencilBuffer = nullptr;
	ID3D11RenderTargetView*	g_pRenderTargetView = nullptr;
	ID3D11DepthStencilView*	g_pDepthStencilView = nullptr;
	
	D3D11_VIEWPORT			g_ScreenViewport;
}

namespace D2D1_And_DirectWrite
{
	ID2D1Factory*			g_pD2DFactory = nullptr;
	ID2D1RenderTarget*		g_pD2DRenderTarget = nullptr;
	ID2D1SolidColorBrush*	g_pD2DSolidBrush_Headline = nullptr;
	ID2D1SolidColorBrush*	g_pD2DSolidBrush_Option_Normal = nullptr;
	ID2D1SolidColorBrush*	g_pD2DSolidBrush_Option_Chosen = nullptr;

	IDWriteFactory*			g_pDWFactory = nullptr;
	IDWriteTextLayout*		g_pDWTextLayout = nullptr;
	IDWriteTextFormat*		g_pDWTextFormat_Headline = nullptr;
	IDWriteTextFormat*		g_pDWTextFormat_Option_Normal = nullptr;
	IDWriteTextFormat*		g_pDWTextFormat_Option_Chosen = nullptr;

	D2D1_COLOR_F			g_FontColor_Headline = D2D1::ColorF(D2D1::ColorF::Cornsilk);
	D2D1_COLOR_F			g_FontColor_Option_Normal = D2D1::ColorF(D2D1::ColorF::OldLace);
	D2D1_COLOR_F			g_FontColor_Option_Chosen = D2D1::ColorF(D2D1::ColorF::Red);

}
HRESULT BaseWindow::Init(HINSTANCE hInstance)
{
	g_hInstance = hInstance;

	WNDCLASSEX wcex;
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)MainWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_hInstance;
	wcex.hIcon			= LoadIcon(g_hInstance, (LPCTSTR)IDI_ICON_SNAKE);
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName	= nullptr;
	wcex.lpszClassName	= L"D3DWndClassName";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON_SNAKE);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return E_FAIL;
	}

	RECT rc = { 0, 0, g_nClientWidth, g_nClientHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int scrWidth = GetSystemMetrics(SM_CXSCREEN);
	int scrHeight = GetSystemMetrics(SM_CYSCREEN);

	g_hMainWnd = CreateWindow(
		L"D3DWndClassName",
		g_wsMainWndCaption.c_str(),
		//WS_OVERLAPPEDWINDOW,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		(scrWidth - width) / 2,
		(scrHeight - height) / 2,
		width,
		height,
		nullptr,
		nullptr,
		g_hInstance,
		nullptr
	);
	if (!g_hMainWnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return E_FAIL;
	}

	ShowWindow(g_hMainWnd, SW_SHOW);
	UpdateWindow(g_hMainWnd);

	return S_OK;
}

float BaseWindow::GetAspectRatio()
{
	return (float)(g_nClientWidth) / g_nClientHeight;
}


HRESULT D3DApplication::Init()
{
	ZeroMemory(&g_ScreenViewport, sizeof(D3D11_VIEWPORT));

#pragma region Create Device and Context
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,					 // default adapter
		g_D3DDriverType,
		0,					 // no software device
		createDeviceFlags,
		0, 0,				 // default feature level array
		D3D11_SDK_VERSION,
		&g_pD3DDevice,
		&featureLevel,
		&g_pD3DImmediateContext
	);
	
	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return E_FAIL;
	}
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return E_FAIL;
	}
#pragma endregion 

#pragma region Check 4X MSAA quality support
	HR(g_pD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &g_4xMsaaQuality));
	assert(g_4xMsaaQuality > 0);
#pragma endregion

#pragma region Creat Swap Chain
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width						= BaseWindow::g_nClientWidth;
	sd.BufferDesc.Height					= BaseWindow::g_nClientHeight;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferDesc.Format					= DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	if (g_Enable4xMsaa)
	{
		sd.SampleDesc.Count		= 4;
		sd.SampleDesc.Quality	= g_4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count		= 1;
		sd.SampleDesc.Quality	= 0;
	}
	sd.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount	= 1;
	sd.OutputWindow = BaseWindow::g_hMainWnd;
	sd.Windowed		= true;
	sd.SwapEffect	= DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags		= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	IDXGIDevice *dxgiDevice = nullptr;
	HR(g_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter *dxgiAdapter = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory *dxgiFactory = nullptr;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));
	HR(dxgiFactory->CreateSwapChain(g_pD3DDevice, &sd, &g_pSwapChain));

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);
#pragma endregion
	
	BaseUpdate();
	return S_OK;
}

void D3DApplication::BaseUpdate()
{
	assert(g_pD3DImmediateContext);
	assert(g_pD3DDevice);
	assert(g_pSwapChain);

	ReleaseCOM(D2D1_And_DirectWrite:: g_pD2DRenderTarget);
	ReleaseCOM(g_pRenderTargetView);
	ReleaseCOM(g_pDepthStencilView);
	ReleaseCOM(g_pDepthStencilBuffer);

	HR(
		g_pSwapChain->ResizeBuffers(
			1, 
			BaseWindow::g_nClientWidth, 
			BaseWindow::g_nClientHeight, 
			DXGI_FORMAT_R8G8B8A8_UNORM, 
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)
	);

#pragma region Create Render TargetView
	ID3D11Texture2D *backBuffer;
	HR(g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(g_pD3DDevice->CreateRenderTargetView(backBuffer, 0, &g_pRenderTargetView));
	ReleaseCOM(backBuffer);
#pragma endregion

#pragma region Create Depth/Stencil Buffer and View
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = BaseWindow::g_nClientWidth;
	depthStencilDesc.Height = BaseWindow::g_nClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (g_Enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = g_4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(g_pD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &g_pDepthStencilBuffer));
	HR(g_pD3DDevice->CreateDepthStencilView(g_pDepthStencilBuffer, 0, &g_pDepthStencilView));
#pragma endregion

	g_pD3DImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);

#pragma region Set the Viewport
	g_ScreenViewport.TopLeftX = 0;
	g_ScreenViewport.TopLeftY = 0;
	g_ScreenViewport.Width = (float)BaseWindow::g_nClientWidth;
	g_ScreenViewport.Height = (float)BaseWindow::g_nClientHeight;
	g_ScreenViewport.MinDepth = 0.0f;
	g_ScreenViewport.MaxDepth = 1.0f;
#pragma endregion

	g_pD3DImmediateContext->RSSetViewports(1, &g_ScreenViewport);
}

void D3DApplication::ClearDevice()
{
	ReleaseCOM(g_pRenderTargetView);
	ReleaseCOM(g_pDepthStencilView);
	ReleaseCOM(g_pSwapChain);
	ReleaseCOM(g_pDepthStencilBuffer);

	if (g_pD3DImmediateContext)
		g_pD3DImmediateContext->ClearState();

	ReleaseCOM(g_pD3DImmediateContext);
	ReleaseCOM(g_pD3DDevice);
}

HRESULT D2D1_And_DirectWrite::Init()
{
	auto options = D2D1_FACTORY_OPTIONS();
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;

	HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, options, &g_pD2DFactory));
	HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&g_pDWFactory)));
	
	BaseUpdate();

	HR(
		g_pDWFactory->CreateTextFormat(
			L"Jokerman", nullptr, 
			DWRITE_FONT_WEIGHT_NORMAL, 
			DWRITE_FONT_STYLE_NORMAL, 
			DWRITE_FONT_STRETCH_NORMAL, 
			195.0f, L"", 
			&g_pDWTextFormat_Headline)
	);
	HR(
		g_pDWFactory->CreateTextFormat(
			L"Comic Sans MS", nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			40.0f, L"",
			&g_pDWTextFormat_Option_Normal)
	);
	HR(
		g_pDWFactory->CreateTextFormat(
			L"Comic Sans MS", nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			55.0f, L"",
			&g_pDWTextFormat_Option_Chosen)
	);
	
	g_pDWTextFormat_Headline->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	g_pDWTextFormat_Headline->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	g_pDWTextFormat_Option_Normal->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	g_pDWTextFormat_Option_Normal->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	g_pDWTextFormat_Option_Chosen->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	g_pDWTextFormat_Option_Chosen->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	return S_OK;
}

void D2D1_And_DirectWrite::BaseUpdate()
{
	ReleaseCOM(g_pD2DRenderTarget);
	ReleaseCOM(g_pD2DSolidBrush_Headline);
	ReleaseCOM(g_pD2DSolidBrush_Option_Normal);
	ReleaseCOM(g_pD2DSolidBrush_Option_Chosen);

	IDXGISurface *backBufferSurface;
	HR(D3DApplication::g_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(&backBufferSurface)));

	float dpiX;
	float dpiY;
	g_pD2DFactory->GetDesktopDpi(&dpiX, &dpiY);

	auto d2dRTProps = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT, 
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), 
		dpiX, dpiY
	);
	
	HR(g_pD2DFactory->CreateDxgiSurfaceRenderTarget(backBufferSurface, &d2dRTProps, &g_pD2DRenderTarget));
	HR(g_pD2DRenderTarget->CreateSolidColorBrush(g_FontColor_Headline, &g_pD2DSolidBrush_Headline));
	HR(g_pD2DRenderTarget->CreateSolidColorBrush(g_FontColor_Option_Normal, &g_pD2DSolidBrush_Option_Normal));
	HR(g_pD2DRenderTarget->CreateSolidColorBrush(g_FontColor_Option_Chosen, &g_pD2DSolidBrush_Option_Chosen));

	ReleaseCOM(backBufferSurface);
}

void D2D1_And_DirectWrite::ClearDevice()
{
	ReleaseCOM(g_pD2DSolidBrush_Headline);
	ReleaseCOM(g_pD2DSolidBrush_Option_Normal);
	ReleaseCOM(g_pD2DSolidBrush_Option_Chosen);
	ReleaseCOM(g_pD2DRenderTarget);
	ReleaseCOM(g_pD2DFactory);

	ReleaseCOM(g_pDWTextFormat_Headline);
	ReleaseCOM(g_pDWTextFormat_Option_Normal);
	ReleaseCOM(g_pDWTextFormat_Option_Chosen);
	ReleaseCOM(g_pDWTextLayout);
	ReleaseCOM(g_pDWFactory);
}

