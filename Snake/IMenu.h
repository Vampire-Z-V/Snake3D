#pragma once
#include <d2d1.h>
#include <dwrite.h>
#include <Windows.h>
#include <string>
#include <map>
using namespace std;

namespace GameManager
{
	class IMenu
	{
	public:
		IMenu();

		void MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		void StartMenu_UpdateAndRender();
		
	private:
		void StartMenu_Choosing(bool isUP);
		void StartMenu_ChangeState();

	private:
		map<string,pair<wstring, D2D1_RECT_F>> m_OutputText;
		int m_nStartMenu_opt;

	
	};
}

