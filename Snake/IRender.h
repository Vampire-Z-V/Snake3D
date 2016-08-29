#pragma once
#include <d2d1.h>
#include <string>
#include <map>
#include <vector>
#include <Windows.h>
#include <DirectXMath.h>
#include "ICamera.h"
#include "_BaseGameObject.h"
using namespace std;

namespace GameManager
{
	class IRender
	{
	public:
		IRender();
		void SetCamera(ICamera &camera);
		void SetLight(DirectionalLight dirlight[]);

		// Menu
		void DrawStartMenu(
			map<string, pair<wstring, D2D1_RECT_F>> &OutputText,
			int opt
		);

		// Game
		void DrawMainGame_Base();

		void DrawWall(
			vector<XMFLOAT4X4> &gridWorld,
			vector<XMFLOAT4X4> &boxWorld,
			_BaseGameObject::GeometryInfo &gridInfo,
			_BaseGameObject::GeometryInfo &boxInfo,
			ID3D11Buffer *VB,
			ID3D11Buffer *IB,
			vector<ID3D11ShaderResourceView*> &SRV
		);

		void DrawSnake(
			XMFLOAT4X4 &headWorld,
			vector<XMFLOAT4X4> &bodyWorld,
			_BaseGameObject::GeometryInfo &headInfo,
			_BaseGameObject::GeometryInfo &bodyInfo,
			ID3D11Buffer *VB,
			ID3D11Buffer *IB,
			vector<ID3D11ShaderResourceView*> &SRV
		);

		void DrawMainGame_Present();

	private:
		void UpdateBackgroundColor(float **color);

	private:
		vector<DirectX::XMVECTORF32> m_Color_Background;
		ICamera m_Camera;
		DirectionalLight* m_pDirLight;
	};
}
