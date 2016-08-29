#include <Windows.h>
#include "_BaseWindow.h"
#include "GameManager.h"

using namespace D3DApplication;
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void OnResize();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (FAILED(BaseWindow::Init(hInstance)))
	{
		return 0;
	}

	if (FAILED(D3DApplication::Init()))
	{
		D3DApplication::ClearDevice();
		return 0;
	}

	if (FAILED(D2D1_And_DirectWrite::Init()))
	{
		D2D1_And_DirectWrite::ClearDevice();
		return 0;
	}

	if (!GameManager::Init())
	{
		return 0;
	}

	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameManager::g_CTimer.Tick();

			if (!GameManager::g_bPaused)
			{
				GameManager::CalculateFrameStats();
				GameManager::Run();
			}

			if (GameManager::g_bQuit)
			{
				break;
			}
		}
	}

	D3DApplication::ClearDevice();
	D2D1_And_DirectWrite::ClearDevice();

	return (int)msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
#pragma region Base Message Process
	switch (msg)
	{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				GameManager::g_bPaused = true;
				GameManager::g_CTimer.Stop();
			}
			else
			{
				GameManager::g_bPaused = false;
				GameManager::g_CTimer.Start();
			}
			return 0;

		case WM_SIZE:
			BaseWindow::g_nClientWidth = LOWORD(lParam);
			BaseWindow::g_nClientHeight = HIWORD(lParam);
			if (D3DApplication::g_pD3DDevice)
			{
				if (wParam == SIZE_MINIMIZED)
				{
					GameManager::g_bPaused = true;
					BaseWindow::g_bMinimized = true;
					BaseWindow::g_bMaximized = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					GameManager::g_bPaused = false;
					BaseWindow::g_bMinimized = false;
					BaseWindow::g_bMaximized = true;
					OnResize();
				}
				else if (wParam == SIZE_RESTORED)
				{

					if (BaseWindow::g_bMinimized)
					{
						GameManager::g_bPaused = false;
						BaseWindow::g_bMinimized = false;
						OnResize();
					}

					else if (BaseWindow::g_bMaximized)
					{
						GameManager::g_bPaused = false;
						BaseWindow::g_bMaximized = false;
						OnResize();
					}
					else if (BaseWindow::g_bResizing)
					{
						//Do nothing.
					}
					else
					{
						OnResize();
					}
				}
			}
			return 0;

		case WM_ENTERSIZEMOVE:
			GameManager::g_bPaused = true;
			BaseWindow::g_bResizing = true;
			GameManager::g_CTimer.Stop();
			return 0;

		case WM_EXITSIZEMOVE:
			GameManager::g_bPaused = false;
			BaseWindow::g_bResizing = false;
			GameManager::g_CTimer.Start();
			OnResize();
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

			// The WM_MENUCHAR message is sent when a menu is active and the user presses 
			// a key that does not correspond to any mnemonic or accelerator key. 
		case WM_MENUCHAR:
			// Don't beep when we alt-enter.
			return MAKELRESULT(0, MNC_CLOSE);

			// Catch this message so to prevent the window from becoming too small.
		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;
	}
#pragma endregion

	switch (GameManager::g_GameState)
	{
		case GameManager::GAME_STATE::GS_MENU:
			GameManager::g_CMenu.MsgProc(hwnd, msg, wParam, lParam);
			break;
		case GameManager::GAME_STATE::GS_GAMESTART:
			GameManager::g_pCMainGame->MsgProc(hwnd, msg, wParam, lParam);
			break;
		default:
			break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void OnResize()
{
	D3DApplication::BaseUpdate();
	D2D1_And_DirectWrite::BaseUpdate();

	switch (GameManager::g_GameState)
	{	
		case GameManager::GAME_STATE::GS_MENU:
			break;
		case GameManager::GAME_STATE::GS_GAME:
			GameManager::g_pCMainGame->OnResize();
			break;
		default:
			break;
	}
}
