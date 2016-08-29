#pragma once
#include "Objects.h"
#include "ICamera.h"
#include "LightHelper.h"

namespace GameManager
{
	class IMainGame
	{
	public:
		IMainGame(ID3D11Device *device);
		~IMainGame();

		void InitAll();
		void DestroyAll();

		void MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void OnResize();

		void MainGame_UpdateAndRender();

	private:
		void OnMouseMove();
		void OnKeyDown();
		void CollisionDetectionAndInteract();
		void GameOver();

	private:
		Objects				m_GameObjects;
		ICamera				m_CCamera;
		DirectionalLight	m_DirLights[3];

		MathHelper::BOUNDING			m_StaticWall_Grid;
		MathHelper::BOUNDING			m_StaticWall_Box;
		const std::vector<XMFLOAT4X4>*	m_pStaticWallWorld_Grid;
		const std::vector<XMFLOAT4X4>*	m_pStaticWallWorld_Box;

		bool isStart;
	};
}
