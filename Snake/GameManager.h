#pragma once

#include "ITimer.h"
#include "IMenu.h"
#include "IRender.h"
#include "IMainGame.h"

namespace GameManager
{
	enum GAME_STATE
	{
		GS_OPENING_CG	= 0,
		GS_MENU			= 1,
		GS_GAME			= 2,
		GS_RANK			= 3,
		GS_SETTING		= 4,
		GS_QUIT			= 5,

		//in GS_Game state
		GS_GAMESTART_CG = 20,
		GS_GAMESTART	= 21,
		GS_GAMEPAUSE	= 22,
		GS_GAMEOVER		= 23,

		//in GS_GamePause state
		GS_GAMECONTINUE = 210,
		GS_GAMEQUIT		= 211,
	};

	extern GAME_STATE	g_GameState;

	extern ITimer		g_CTimer;
	extern IRender		g_CRender;
	extern IMenu		g_CMenu;
	extern IMainGame*	g_pCMainGame;


	extern bool			g_bPaused;
	extern bool			g_bQuit;

	bool Init();
	void Destroy();
	void Run();

	void CalculateFrameStats();

}