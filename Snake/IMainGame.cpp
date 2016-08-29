#include "GameManager.h"
#include <windowsx.h>
using namespace GameManager;

#define IS_KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

namespace BaseWindow
{
	extern int			g_nClientWidth;
	extern int			g_nClientHeight;
	extern float GetAspectRatio();
}

IMainGame::IMainGame(ID3D11Device *device)
:	m_GameObjects(device)
{
	m_DirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f);
	m_DirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_DirLights[0].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	
	m_DirLights[1].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 0.5f);
	m_DirLights[1].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLights[1].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
	
	m_DirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[2].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_DirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);

	m_StaticWall_Grid = m_GameObjects.wall.GetImpactBox(_BaseGameObject::BUFFER_TYPE::GRID);
	m_StaticWall_Box = m_GameObjects.wall.GetImpactBox(_BaseGameObject::BUFFER_TYPE::BOX);
	m_pStaticWallWorld_Grid = m_GameObjects.wall.GetWorld(_BaseGameObject::BUFFER_TYPE::GRID);
	m_pStaticWallWorld_Box = m_GameObjects.wall.GetWorld(_BaseGameObject::BUFFER_TYPE::BOX);
}


IMainGame::~IMainGame()
{
}

void IMainGame::InitAll()
{
	ShowCursor(false);
	//m_CCamera.SetPosition(0.0f, 100.0f, 0.0f);
	//m_CCamera.SetPosition(0.0f,0.5f, -15.0f);
	//XMFLOAT3 up = { 0.0f,0.0f,1.0f };
	m_GameObjects.snake.Init();
	///m_CCamera.SetPosition(m_GameObjects.snake.GetHeadPos());
	///m_CCamera.SetLook(m_GameObjects.snake.GetHeadLook());
	///m_CCamera.SetUp(m_GameObjects.snake.GetHeadUp());
	///m_CCamera.SetRight(m_GameObjects.snake.GetHeadRight());

	XMFLOAT3 pos = { 0.0f,53.0f,-4.0f };
	//XMFLOAT3 target = { 0.0f,0.0f,0.0f };
	XMFLOAT3 target = m_GameObjects.snake.GetHeadPos();
	XMFLOAT3 right = m_GameObjects.snake.GetHeadRight();
	XMVECTOR s = XMVectorReplicate(1.0f);
	XMVECTOR u = XMLoadFloat3(&m_GameObjects.snake.GetHeadUp());
	XMVECTOR p = XMLoadFloat3(&target);
	XMFLOAT3 currPos;
	XMStoreFloat3(&currPos, XMVectorMultiplyAdd(s, u, p));

	m_CCamera.LookAt(pos,currPos,right);
	m_CCamera.SetLens(0.25f*MathHelper::Pi, BaseWindow::GetAspectRatio(), 1.0f, 1000.0f);
	isStart = false;
}

void IMainGame::DestroyAll()
{
}


void IMainGame::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_NUMPAD0:
					ShowCursor(true);
					break;
				case VK_NUMPAD1:
					ShowCursor(false);
					break;
				case 'A':
					//m_GameObjects.snake.RotateUp(-0.5*XM_PI);
					break;
				case 'D':
					//m_GameObjects.snake.RotateUp(0.5*XM_PI);
					break;
				case VK_SPACE:
					//m_GameObjects.snake.IncreaseBodyLength();
					isStart = true;
					break;
				case VK_RETURN:
					m_GameObjects.snake.IncreaseBodyLength();
					break;
			}
			break;
		case WM_KEYUP:
			break;
		case WM_MOUSEMOVE:
			//MessageBox(hwnd, L"...", L"test", 0);
			//OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
	}
}

void IMainGame::OnResize()
{
}

void IMainGame::MainGame_UpdateAndRender()
{
	if(isStart)
		OnKeyDown();
		//m_GameObjects.snake.Walk(0.001);
		OnMouseMove();
		//m_GameObjects.snake.RotateUp(-0.5*XM_PI);

	//m_GameObjects.snake.SetSnakeHeadPosition(m_CCamera.GetPosition());
	m_GameObjects.wall.Update();
	m_GameObjects.fruit.Update();
	m_GameObjects.snake.Update();

	/*if (IS_KEYDOWN(VK_ADD))
	{
		m_GameObjects.snake.IncreaseBodyLength();
	}*/

	//XMFLOAT3 pos = m_CCamera.GetPosition();
	//XMFLOAT3 target = m_GameObjects.snake.GetHeadPos();
	//XMFLOAT3 right = m_GameObjects.snake.GetHeadRight();
	//
	//m_CCamera.LookAt(pos, target, right);
	m_CCamera.UpdateViewMatrix();
	CollisionDetectionAndInteract();

	g_CRender.SetCamera(m_CCamera);
	g_CRender.SetLight(m_DirLights);
	
	g_CRender.DrawMainGame_Base();

	m_GameObjects.wall.Render();
	m_GameObjects.fruit.Render();
	m_GameObjects.snake.Render();

	g_CRender.DrawMainGame_Present();
}

void IMainGame::OnMouseMove()
{
	float dt = g_CTimer.DeltaTime();

	static const int wndWidth = BaseWindow::g_nClientWidth;
	static const int wndHeight = BaseWindow::g_nClientHeight;
	static const POINT wnd = {	
		(GetSystemMetrics(SM_CXSCREEN) - wndWidth) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - wndHeight) / 2 
	};
	static const RECT wndArea = {
		wnd.x,
		wnd.y,
		wnd.x + wndWidth - 1,
		wnd.y + wndHeight - 1
	};

	static POINT lastCursorPos = { 0,0 };
	static POINT currentCursorPos = { 0,0 };

	lastCursorPos = currentCursorPos;
	GetCursorPos(&currentCursorPos);

	if (currentCursorPos.x >= wndArea.right)
	{
		SetCursorPos(wndArea.left, currentCursorPos.y);
		lastCursorPos = { wndArea.left,currentCursorPos.y };
		currentCursorPos = lastCursorPos;
	}
	else if (currentCursorPos.x <= wndArea.left)
	{
		SetCursorPos(wndArea.right, currentCursorPos.y);
		lastCursorPos = { wndArea.right,currentCursorPos.y };
		currentCursorPos = lastCursorPos;
	}

	if (currentCursorPos.y >= wndArea.bottom)
	{
		SetCursorPos(currentCursorPos.x, wndArea.top);
		lastCursorPos = { currentCursorPos.x,wndArea.top };
		currentCursorPos = lastCursorPos;
	}
	else if (currentCursorPos.y <= wndArea.top)
	{
		SetCursorPos(currentCursorPos.x, wndArea.bottom);
		lastCursorPos = { currentCursorPos.x,wndArea.bottom };
		currentCursorPos = lastCursorPos;
	}
	if (isStart)
	{
		float dx = XMConvertToRadians(0.1f*static_cast<float>(currentCursorPos.x - lastCursorPos.x));
		//float dy = XMConvertToRadians(0.1f*static_cast<float>(currentCursorPos.y - lastCursorPos.y));
		//m_CCamera.Pitch(dy);
		//m_CCamera.RotateY(dx);
		//m_CCamera.RotateUp(dx);
		m_GameObjects.snake.RotateUp(dx*dt*2000.0f);
		m_CCamera.RotateAxis(m_GameObjects.snake.GetHeadUp(), m_GameObjects.snake.GetHeadPos(), dx*dt*2000.0f);
	}
}

void IMainGame::OnKeyDown()
{
	float dt = g_CTimer.DeltaTime();
	float dv = 50.0f*dt;
	//m_CCamera.Walk(dv);
	XMFLOAT3 look = m_CCamera.GetLook();
	m_GameObjects.snake.Walk(0.2f*dv);
	m_CCamera.SetLook(m_GameObjects.snake.GetHeadLook());
	m_CCamera.Walk(0.2f*dv);
	m_CCamera.SetLook(look);

	//float upSpeed = XMConvertToRadians(0.015f);
	float rotateSpeed = XMConvertToRadians(dv);
	if (IS_KEYDOWN('W'))
	{
		//m_GameObjects.snake.Walk(dv);
		m_GameObjects.snake.RotateRight(-rotateSpeed);
		m_CCamera.RotateAxis(m_GameObjects.snake.GetHeadRight(), m_GameObjects.snake.GetHeadPos(), -rotateSpeed);
		//m_CCamera.RotateRight(-rotateSpeed);
	}
	else if (IS_KEYDOWN('S'))
	{
		//m_GameObjects.snake.Walk(-dv);
		m_GameObjects.snake.RotateRight(rotateSpeed);
		m_CCamera.RotateAxis(m_GameObjects.snake.GetHeadRight(), m_GameObjects.snake.GetHeadPos(), rotateSpeed);
		//m_CCamera.RotateRight(rotateSpeed);
	}
	if (IS_KEYDOWN('A'))
	{
		m_GameObjects.snake.RotateLook(rotateSpeed);
		m_CCamera.RotateAxis(m_GameObjects.snake.GetHeadLook(), m_GameObjects.snake.GetHeadPos(), rotateSpeed);
		//m_GameObjects.snake.RotateUp(-0.5*XM_PI);
	}
	else if (IS_KEYDOWN('D'))
	{
		m_GameObjects.snake.RotateLook(-rotateSpeed);
		m_CCamera.RotateAxis(m_GameObjects.snake.GetHeadLook(), m_GameObjects.snake.GetHeadPos(), -rotateSpeed);
		//m_GameObjects.snake.RotateUp(0.5*XM_PI);
	}
	if (IS_KEYDOWN('Q'))
	{
		//m_GameObjects.snake.Walk(dv);
	}
	if (IS_KEYDOWN('E'))
	{
		//m_GameObjects.snake.Walk(-dv);
	}

	//if (IS_KEYDOWN(VK_UP))
	//{
	//	m_CCamera.Walk(dv);
	//	//m_CCamera.RotateRight(-upSpeed);
	//}
	//if (IS_KEYDOWN(VK_DOWN))
	//{
	//	m_CCamera.Walk(-dv);
	//	//m_CCamera.RotateRight(upSpeed);
	//}
	//if (IS_KEYDOWN(VK_LEFT))
	//{
	//	m_CCamera.Strafe(-dv);
	//	//m_CCamera.RotateLook(rotateSpeed);
	//}
	//if (IS_KEYDOWN(VK_RIGHT))
	//{
	//	m_CCamera.Strafe(dv);
	//	//m_CCamera.RotateLook(-rotateSpeed);
	//}
}

void GameManager::IMainGame::CollisionDetectionAndInteract()
{
	/// We haven created every objects' impactbox.
	/// First, get the target's world matrix and inverse it.
	/// Then, transform the object(the snake head)'s coord with the matrix,
	///	which makes the object's coord about the basic coordinate system, 
	///	because every impactboxs are base on this system.
	/// Finally, do the collision detection.


	MathHelper::BOUNDING snakeHeadImpactBox = m_GameObjects.snake.GetImpactBox(Snake::SNAKE_TYPE::HEAD);
	XMVECTOR snakeHeadPos = XMLoadFloat3(&snakeHeadImpactBox.Sphere.CenterPos);

	// Firstly,
	// Obj: Snake's head
	// Target: Wall (Grid)
	for (size_t i = 0; i < m_pStaticWallWorld_Grid->size(); i++)
	{
		XMMATRIX gridWallWorldTemp = XMLoadFloat4x4(&(*m_pStaticWallWorld_Grid)[i]);
		gridWallWorldTemp = XMMatrixInverse(&XMMatrixDeterminant(gridWallWorldTemp), gridWallWorldTemp);

		XMVECTOR snakeHeadPos_Changed = XMVector3TransformCoord(snakeHeadPos, gridWallWorldTemp);

		MathHelper::BOUNDING::SPHERE snakeHeadImpactBox_Changed;
		XMStoreFloat3(&snakeHeadImpactBox_Changed.CenterPos, snakeHeadPos_Changed);
		snakeHeadImpactBox_Changed.Radius = snakeHeadImpactBox.Sphere.Radius;

		if (MathHelper::isCollided(snakeHeadImpactBox_Changed, m_StaticWall_Grid.Box))
		{
			/*switch (i)
			{
				case 1:
					MessageBox(0, L"isCollided!-1", 0, 0);
					return;

				case 2:
					MessageBox(0, L"isCollided!-2", 0, 0);
					return;
				case 3:
					MessageBox(0, L"isCollided!-3", 0, 0);
					return;
				case 4:
					MessageBox(0, L"isCollided!-4", 0, 0);
					return;
				default:
					break;
			}
			MessageBox(0, L"isCollided!", 0, 0);
			return;*/
			g_GameState = GS_MENU;
			ShowCursor(true);
			//GameOver();
			return;
		}
	}

	// Secondly,
	// Obj: Snake's head
	// Target: Wall (Box)
	for (size_t i = 0; i<m_pStaticWallWorld_Box->size(); i++)
	{
		XMMATRIX boxWallWorldTemp = XMLoadFloat4x4(&(*m_pStaticWallWorld_Box)[i]);
		boxWallWorldTemp = XMMatrixInverse(&XMMatrixDeterminant(boxWallWorldTemp), boxWallWorldTemp);

		XMVECTOR snakeHeadPos_Changed = XMVector3TransformCoord(snakeHeadPos, boxWallWorldTemp);

		MathHelper::BOUNDING::SPHERE snakeHeadImpactBox_Changed;
		XMStoreFloat3(&snakeHeadImpactBox_Changed.CenterPos, snakeHeadPos_Changed);
		snakeHeadImpactBox_Changed.Radius = snakeHeadImpactBox.Sphere.Radius;

		if (MathHelper::isCollided(snakeHeadImpactBox_Changed, m_StaticWall_Box.Box))
		{
			g_GameState = GS_MENU;
			ShowCursor(true);
			return;
		}
	}

}

void GameManager::IMainGame::GameOver()
{
	static bool first = true;

	static XMFLOAT3 cameraUpRec;
	static XMFLOAT3 cameraRightRec;
	static XMFLOAT3 cameraLookRec;

	if (first)
	{
		cameraUpRec = m_CCamera.GetUp();
		cameraRightRec = m_CCamera.GetRight();
		cameraLookRec = m_CCamera.GetLook();
	}

	static float dx_1 = 100.0f;
	static float dx_2 = -200.0f;
	static float dx_3 = 150.0f;
	static float dx_4 = -100.0f;
	static int count = 0;

	switch (count)
	{

		case 0:
			m_CCamera.RotateUp(dx_1);
			count++;
			first = false;
			break;
		case 1:
			m_CCamera.RotateUp(dx_2);
			count++;
			first = false;
			break;
		case 2:
			m_CCamera.RotateUp(dx_3);
			count++;
			first = false;
			break;
		case 3:
			m_CCamera.RotateUp(dx_4);
			count++;
			first = false;
			break;
		default:
			m_CCamera.SetUp(cameraUpRec);
			m_CCamera.SetLook(cameraLookRec);
			m_CCamera.SetRight(cameraRightRec);
			first = true;
			count = 0;
			break;
	}
}

