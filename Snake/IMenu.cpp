#include "GameManager.h"
//#include "_BaseWindow.h"
//using namespace D2D1_And_DirectWrite;

GameManager::IMenu::IMenu()
{
	m_nStartMenu_opt = 1;
	m_OutputText.insert(pair<string, pair<wstring, D2D1_RECT_F>>("Headline", 
																	{ L"SNAKE",		D2D1::RectF(280, 50, 1000, 300) }));

	m_OutputText.insert(pair<string, pair<wstring, D2D1_RECT_F>>("StartMenu_opt_1", 
																	{ L"New Game",	D2D1::RectF(460, 330, 820, 500) }));
	m_OutputText.insert(pair<string, pair<wstring, D2D1_RECT_F>>("StartMenu_opt_2", 
																	{ L"Rank",		D2D1::RectF(460, 370, 820, 600) }));
	m_OutputText.insert(pair<string, pair<wstring, D2D1_RECT_F>>("StartMenu_opt_3", 
																	{ L"Setting",	D2D1::RectF(460, 410, 820, 700) }));
	m_OutputText.insert(pair<string, pair<wstring, D2D1_RECT_F>>("StartMenu_opt_4", 
																	{ L"Quit Game",	D2D1::RectF(460, 450, 820, 800) }));
}

void GameManager::IMenu::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case VK_UP:
					StartMenu_Choosing(true);
					break;
				case VK_DOWN:
					StartMenu_Choosing(false);
					break;

				case VK_RETURN:
					StartMenu_ChangeState();
					break;
			}
			break;
		case WM_KEYUP:
			break;
	}
}

void GameManager::IMenu::StartMenu_UpdateAndRender()
{
	g_CRender.DrawStartMenu(m_OutputText, m_nStartMenu_opt);
}

void GameManager::IMenu::StartMenu_Choosing(bool isUP)
{
	if (isUP)
	{
		m_nStartMenu_opt--;
		m_nStartMenu_opt = (m_nStartMenu_opt < 1 ? 1 : m_nStartMenu_opt);
	}
	else
	{
		m_nStartMenu_opt++;
		m_nStartMenu_opt = (m_nStartMenu_opt > 4 ? 4 : m_nStartMenu_opt);
	}
}

void GameManager::IMenu::StartMenu_ChangeState()
{
	switch (m_nStartMenu_opt)
	{
		case 1:
			g_GameState = GS_GAME;
			break;
		case 2:
			//g_GameState = GS_RANK;
			break;
		case 3:
			//g_GameState = GS_SETTING;
			break;
		case 4:
			g_GameState = GS_QUIT;
			break;
	}
}
