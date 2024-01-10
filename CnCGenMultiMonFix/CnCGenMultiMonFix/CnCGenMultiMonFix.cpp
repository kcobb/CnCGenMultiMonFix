// CnCGenMultiMonFix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <windows.h>

using namespace std;


HWND FindVisibleWindow(_TCHAR* lpClass, _TCHAR* lpTitle)
{
	HWND hwndNext = nullptr;
	HWND hwnd = nullptr;

	do
	{
		hwnd = FindWindowEx(NULL, hwndNext, lpClass, lpTitle);
		if (hwnd && IsWindowVisible(hwnd))
			break;

		hwndNext = hwnd;
	} while (hwnd != nullptr);

	return hwnd;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int intAppCount = 2;
	int intLoopCount = 0;
	const char spinner[] = { '|', '/', '-', '\\' };
	wchar_t ** wcWinNameText;
	wcWinNameText = new wchar_t*[intAppCount];
	wcWinNameText[0] = L"Command & Conquer Generals";
	wcWinNameText[1] = L"Command & Conquer (TM) Generals Zero Hour";

	SetConsoleTitleA("C&C Generals - MultiMonitor Lock");
	//string strCopyRight("(c) Copyright 2022 - github.com/kcobb");
	//string strNotice("Searching for C&C Game Window...\n" + strCopyRight + "\n");
	//cout << strNotice;

	HWND hwndNext = nullptr;
	HWND hwnd = nullptr;

	HWND window=0;
	RECT r;
	BOOL bWinLock = false;



	while (true){
		intLoopCount++;
		std::cout << "searching for C&C game window...                           \r";
		for (int i = 0; i < intAppCount; i++) {
			window = FindVisibleWindow(NULL, wcWinNameText[i]);
			if (window != 0) {
				bWinLock = true;
				break;
			}
		}
		if (bWinLock && window != 0) {
			//cout << "game window locked\n";
			std::cout << "game window locked " << spinner[intLoopCount % 4] << "                                        \r";
			std::cout.flush();
			GetWindowRect(window, &r);
			ClipCursor(&r);
			bWinLock = true;
		}
		if (bWinLock && window == 0){
			std::cout << "game window lost focus, closing app                        \r";
			std::cout.flush();
			ClipCursor(nullptr);
			bWinLock = false;
			window = 0;
			Sleep(5000);
			exit(0);
		}
		Sleep(1000);
	}
}

