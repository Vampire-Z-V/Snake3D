#include "GameManager.h"
#include "_BaseWindow.h"
#include "InputLayouts.h"
#include "Effects.h"
#include "RenderStates.h"
#include <sstream>

namespace GameManager
{
	GAME_STATE	g_GameState;

	ITimer		g_CTimer;
	IMenu		g_CMenu;
	IRender		g_CRender;
	IMainGame*	g_pCMainGame;

	bool		g_bPaused;
	bool		g_bQuit;
}

bool GameManager::Init()
{
	g_GameState = GS_OPENING_CG;
	g_bPaused = false;
	g_bQuit = false;

	g_CTimer.Reset();
	g_pCMainGame = new IMainGame(D3DApplication::g_pD3DDevice);

	Effects::InitAll(D3DApplication::g_pD3DDevice);
	InputLayouts::InitAll(D3DApplication::g_pD3DDevice);
	RenderStates::InitAll(D3DApplication::g_pD3DDevice);

	return true;
}

void GameManager::Destroy()
{
	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();

	SafeDelete(g_pCMainGame);
}

void GameManager::Run()
{
	switch (g_GameState)
	{
		case GS_OPENING_CG:
			g_GameState = GS_MENU;
			break;

		case GS_MENU:
			g_CMenu.StartMenu_UpdateAndRender();
			break;

		case GS_GAME:
			g_pCMainGame->InitAll();
			g_GameState = GS_GAMESTART_CG;
			break;
		case GS_GAMESTART_CG:
			g_GameState = GS_GAMESTART;
			break;
		case GS_GAMESTART:
			g_pCMainGame->MainGame_UpdateAndRender();
			break;
		case GS_GAMEPAUSE:
			break;
		case GS_GAMEOVER:
			break;

		case GS_RANK:
			break;
		case GS_SETTING:
			break;
		case GS_QUIT:
			g_bQuit = true;
			break;
		default:
			break;
	}
}

void GameManager::CalculateFrameStats()
{
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;

	if ((g_CTimer.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCount;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << BaseWindow::g_wsMainWndCaption << L"    "
			<< L"FPS: " << fps;
		SetWindowText(BaseWindow::g_hMainWnd, outs.str().c_str());

		frameCount = 0;
		timeElapsed += 1.0f;
	}
}