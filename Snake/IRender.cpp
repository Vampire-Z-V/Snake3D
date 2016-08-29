#include "GameManager.h"
#include "_BaseWindow.h"
#include "MathHelper.h"
#include "Effects.h"
#include "InputLayouts.h"
#include "RenderStates.h"
#include <DirectXColors.h>

using namespace D3DApplication;
using namespace D2D1_And_DirectWrite;
using namespace GameManager;

IRender::IRender()
{
	m_Color_Background.clear();
	m_Color_Background.push_back(DirectX::Colors::LightCoral);
	m_Color_Background.push_back(DirectX::Colors::DarkSalmon);
	m_Color_Background.push_back(DirectX::Colors::Salmon);
	m_Color_Background.push_back(DirectX::Colors::LightSalmon);

}

void IRender::SetCamera(ICamera & camera)
{
	m_Camera = camera;
}

void IRender::SetLight(DirectionalLight dirlight[])
{
	m_pDirLight = dirlight;
}

/****** Public Function ***************************************/

void IRender::DrawStartMenu(map<string, pair<wstring, D2D1_RECT_F>> &OutputText, int opt)
{
	float *pBackgroundColor;

	UpdateBackgroundColor(&pBackgroundColor);
	g_pD3DImmediateContext->ClearRenderTargetView(g_pRenderTargetView, pBackgroundColor);


	g_pD2DRenderTarget->BeginDraw();
	//g_pD2DRenderTarget->DrawLine(D2D1::Point2F(BaseWindow::g_nClientWidth / 2, 0), D2D1::Point2F(BaseWindow::g_nClientWidth / 2, BaseWindow::g_nClientHeight), g_pD2DSolidBrush_Option_Chosen);
	auto iter = OutputText.find("Headline");
	if (iter != OutputText.end())
	{
		g_pD2DRenderTarget->DrawText(iter->second.first.c_str(), iter->second.first.size(), g_pDWTextFormat_Headline, iter->second.second, g_pD2DSolidBrush_Headline);
	}

	ID2D1SolidColorBrush **Brush_opt_1, **Brush_opt_2, **Brush_opt_3, **Brush_opt_4;
	IDWriteTextFormat **Format_opt_1, **Format_opt_2, **Format_opt_3, **Format_opt_4;

	Brush_opt_1 = Brush_opt_2 = Brush_opt_3 = Brush_opt_4 = &g_pD2DSolidBrush_Option_Normal;
	Format_opt_1 = Format_opt_2 = Format_opt_3 = Format_opt_4 = &g_pDWTextFormat_Option_Normal;
	switch (opt)
	{
		case 1: Brush_opt_1 = &g_pD2DSolidBrush_Option_Chosen; Format_opt_1 = &g_pDWTextFormat_Option_Chosen; break;
		case 2: Brush_opt_2 = &g_pD2DSolidBrush_Option_Chosen; Format_opt_2 = &g_pDWTextFormat_Option_Chosen; break;
		case 3: Brush_opt_3 = &g_pD2DSolidBrush_Option_Chosen; Format_opt_3 = &g_pDWTextFormat_Option_Chosen; break;
		case 4: Brush_opt_4 = &g_pD2DSolidBrush_Option_Chosen; Format_opt_4 = &g_pDWTextFormat_Option_Chosen; break;
	}

	iter = OutputText.find("StartMenu_opt_1");
	if (iter != OutputText.end())
	{
		g_pD2DRenderTarget->DrawText(iter->second.first.c_str(), iter->second.first.size(), *Format_opt_1, iter->second.second, *Brush_opt_1);
	}
	iter = OutputText.find("StartMenu_opt_2");
	if (iter != OutputText.end())
	{
		g_pD2DRenderTarget->DrawText(iter->second.first.c_str(), iter->second.first.size(), *Format_opt_2, iter->second.second, *Brush_opt_2);
	}
	iter = OutputText.find("StartMenu_opt_3");
	if (iter != OutputText.end())
	{
		g_pD2DRenderTarget->DrawText(iter->second.first.c_str(), iter->second.first.size(), *Format_opt_3, iter->second.second, *Brush_opt_3);
	}
	iter = OutputText.find("StartMenu_opt_4");
	if (iter != OutputText.end())
	{
		g_pD2DRenderTarget->DrawText(iter->second.first.c_str(), iter->second.first.size(), *Format_opt_4, iter->second.second, *Brush_opt_4);
	}
	g_pD2DRenderTarget->EndDraw();
	
	g_pSwapChain->Present(0, 0);
}

void IRender::DrawMainGame_Base()
{
	g_pD3DImmediateContext->ClearRenderTargetView(g_pRenderTargetView, reinterpret_cast<const float*>(&DirectX::Colors::Black));
	g_pD3DImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	g_pD3DImmediateContext->IASetInputLayout(InputLayouts::BasicVertex);
	g_pD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pD3DImmediateContext->RSSetState(RenderStates::pNoCullRS);

	Effects::pBasicFX->SetDirLights(m_pDirLight);
	Effects::pBasicFX->SetEyePosW(m_Camera.GetPosition());
}

void IRender::DrawWall(
	vector<XMFLOAT4X4> &gridWorld,
	vector<XMFLOAT4X4> &boxWorld,
	_BaseGameObject::GeometryInfo &gridInfo,
	_BaseGameObject::GeometryInfo &boxInfo,
	ID3D11Buffer *VB,
	ID3D11Buffer *IB,
	vector<ID3D11ShaderResourceView*> &SRV
)
{
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;

	XMMATRIX viewProj	= m_Camera.ViewProj();

	UINT stride = sizeof(Vertex::BasicVertex);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	const_cast<ID3DX11EffectTechnique*>(Effects::pBasicFX->GetLight3TexTech())->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		g_pD3DImmediateContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
		g_pD3DImmediateContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the grid.
		size_t num_of_grid = gridWorld.size();
		for (size_t i = 0; i < num_of_grid; ++i)
		{
			world = XMLoadFloat4x4(&gridWorld[i]);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*viewProj;

			Effects::pBasicFX->SetWorld(world);
			Effects::pBasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::pBasicFX->SetWorldViewProj(worldViewProj);
			Effects::pBasicFX->SetTexTransform(XMMatrixScaling(50.0f, 50.0f, 1.0f));
			Effects::pBasicFX->SetMaterial(gridInfo.Material);
			Effects::pBasicFX->SetDiffuseMap(SRV[i]);

			const_cast<ID3DX11EffectTechnique*>(Effects::pBasicFX->GetLight3TexTech())->GetPassByIndex(p)->Apply(0, g_pD3DImmediateContext);
			g_pD3DImmediateContext->DrawIndexed(gridInfo.IndexCount, gridInfo.IndexOffset, gridInfo.VertexOffset);
		}

		// Draw the box.
		size_t num_of_box = boxWorld.size();
		for (size_t i = 0; i < num_of_box; ++i)
		{
			world = XMLoadFloat4x4(&boxWorld[i]);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*viewProj;

			Effects::pBasicFX->SetWorld(world);
			Effects::pBasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::pBasicFX->SetWorldViewProj(worldViewProj);
			Effects::pBasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
			Effects::pBasicFX->SetMaterial(boxInfo.Material);
			Effects::pBasicFX->SetDiffuseMap(SRV[3]);

			const_cast<ID3DX11EffectTechnique*>(Effects::pBasicFX->GetLight3TexTech())->GetPassByIndex(p)->Apply(0, g_pD3DImmediateContext);
			g_pD3DImmediateContext->DrawIndexed(boxInfo.IndexCount, boxInfo.IndexOffset, boxInfo.VertexOffset);
		}
	}
}
void GameManager::IRender::DrawSnake(
	XMFLOAT4X4 & headWorld, 
	vector<XMFLOAT4X4>& bodyWorld, 
	_BaseGameObject::GeometryInfo & headInfo, 
	_BaseGameObject::GeometryInfo & bodyInfo, 
	ID3D11Buffer * VB, 
	ID3D11Buffer * IB, 
	vector<ID3D11ShaderResourceView*>& SRV
)
{
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;

	XMMATRIX viewProj = m_Camera.ViewProj();

	UINT stride = sizeof(Vertex::BasicVertex);
	UINT offset = 0;

	D3DX11_TECHNIQUE_DESC techDesc;
	const_cast<ID3DX11EffectTechnique*>(Effects::pBasicFX->GetLight3TexTech())->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		g_pD3DImmediateContext->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
		g_pD3DImmediateContext->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);

		// Draw the head.

		world = XMLoadFloat4x4(&headWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*viewProj;

		Effects::pBasicFX->SetWorld(world);
		Effects::pBasicFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::pBasicFX->SetWorldViewProj(worldViewProj);
		Effects::pBasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Effects::pBasicFX->SetMaterial(headInfo.Material);
		Effects::pBasicFX->SetDiffuseMap(SRV[0]);

		const_cast<ID3DX11EffectTechnique*>(Effects::pBasicFX->GetLight3TexTech())->GetPassByIndex(p)->Apply(0, g_pD3DImmediateContext);
		g_pD3DImmediateContext->DrawIndexed(headInfo.IndexCount, headInfo.IndexOffset, headInfo.VertexOffset);


		// Draw the bodies
		size_t num_of_body = bodyWorld.size() - 1;
		for (size_t i = 0; i < num_of_body; ++i)
		{
			world = XMLoadFloat4x4(&bodyWorld[i]);
			worldInvTranspose = MathHelper::InverseTranspose(world);
			worldViewProj = world*viewProj;

			Effects::pBasicFX->SetWorld(world);
			Effects::pBasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::pBasicFX->SetWorldViewProj(worldViewProj);
			Effects::pBasicFX->SetTexTransform(XMMatrixScaling(1.0f, 1.0f, 1.0f));
			Effects::pBasicFX->SetMaterial(bodyInfo.Material);
			Effects::pBasicFX->SetDiffuseMap(SRV[1]);

			const_cast<ID3DX11EffectTechnique*>(Effects::pBasicFX->GetLight3TexTech())->GetPassByIndex(p)->Apply(0, g_pD3DImmediateContext);
			g_pD3DImmediateContext->DrawIndexed(bodyInfo.IndexCount, bodyInfo.IndexOffset, bodyInfo.VertexOffset);
		}
	}
}
void IRender::DrawMainGame_Present()
{
	HR(g_pSwapChain->Present(0, 0));
}
/****** Private Function ***************************************/

void IRender::UpdateBackgroundColor(float **color)
{
	static int color_cnt = m_Color_Background.size();
	static int color_opt = rand() % color_cnt;
	static float counter = 0.0f;

	static int nextColorIndex = (color_opt + 1 >= color_cnt ? 0 : color_opt + 1);
	static DirectX::XMFLOAT4 currColorTemp, nextColorTemp;
	static float base_SlewSpeed = 1.0f / 4000.0f;

	if (counter == 0.0f)
	{
		DirectX::XMStoreFloat4(&currColorTemp, m_Color_Background[color_opt]);
		DirectX::XMStoreFloat4(&nextColorTemp, m_Color_Background[nextColorIndex]);
		counter++;
		*color = reinterpret_cast<float*>(&m_Color_Background[color_opt]);
	}
	else
	{
		static DirectX::XMFLOAT4 ColorTemp;

		float SlewSpeed = MathHelper::Min(base_SlewSpeed*counter, 1.0f);
		counter++;

		ColorTemp.x = MathHelper::Lerp(currColorTemp.x, nextColorTemp.x, SlewSpeed);
		ColorTemp.y = MathHelper::Lerp(currColorTemp.y, nextColorTemp.y, SlewSpeed);
		ColorTemp.z = MathHelper::Lerp(currColorTemp.z, nextColorTemp.z, SlewSpeed);
		ColorTemp.w = 1.0f;

		if (DirectX::XMVector4Equal(DirectX::XMLoadFloat4(&ColorTemp), DirectX::XMLoadFloat4(&nextColorTemp)))
		{
			color_opt = nextColorIndex;
			counter = 0.0f;
			nextColorIndex = (color_opt + 1 >= color_cnt ? 0 : color_opt + 1);
		}

		*color = reinterpret_cast<float*>(&ColorTemp);
	}
}
