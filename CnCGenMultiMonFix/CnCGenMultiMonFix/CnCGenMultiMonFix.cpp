// CnCGenMultiMonFix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sys/stat.h>
#include <fstream>
#include <codecvt>
#include <cstdio>
#include <string>
#include <algorithm> 
#include <cctype>
#include <iostream>
#include <windows.h>

using namespace std;

inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

inline bool exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

std::string utf16ToUtf8(const std::wstring& utf16Str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(utf16Str);
}

std::wstring utf8ToUtf16(const std::string& utf8Str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(utf8Str);
}

std::wstring ReadRegistryValue(HKEY hKey, const wchar_t* subKey, const wchar_t* valueName) {
	HKEY key;
	std::wstring result;

	// Open the registry key
	if (RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &key) == ERROR_SUCCESS) {
		DWORD dataSize = 0;
		DWORD dataType;

		// Query the size and type of the registry value
		if (RegQueryValueEx(key, valueName, 0, &dataType, nullptr, &dataSize) == ERROR_SUCCESS) {
			if (dataType == REG_SZ) {
				// Allocate buffer for the string value
				std::wstring buffer(dataSize / sizeof(wchar_t), L'\0');

				// Read the string value
				if (RegQueryValueEx(key, valueName, 0, nullptr, reinterpret_cast<LPBYTE>(&buffer[0]), &dataSize) == ERROR_SUCCESS) {
					result = buffer;
				}
			}
		}

		// Close the registry key
		RegCloseKey(key);
	}

	return result;
}

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
	std::string CnCFullGeneralProg;
	std::string app;

	wchar_t ** wcWinNameText;
	wcWinNameText = new wchar_t*[intAppCount];
	wcWinNameText[0] = L"Command & Conquer Generals";
	wcWinNameText[1] = L"Command & Conquer (TM) Generals Zero Hour";

	SetConsoleTitleA("C&C Generals - MultiMonitor Lock");

	HWND hwndNext = nullptr;
	HWND hwnd = nullptr;

	HWND window=0;
	RECT r;
	BOOL bWinLock = false;

	std::wstring RegInstallPath = ReadRegistryValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\WOW6432Node\\Electronic Arts\\EA Games\\Command and Conquer Generals Zero Hour", L"InstallPath");
	if (RegInstallPath.empty()) {
		RegInstallPath = ReadRegistryValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Electronic Arts\\EA Games\\Command and Conquer Generals Zero Hour", L"InstallPath");
	}
	if (!RegInstallPath.empty()) {
		CnCFullGeneralProg = utf16ToUtf8(RegInstallPath.substr(0, RegInstallPath.length()-1));
		CnCFullGeneralProg.append("\\generals.exe");
	}
	
	if (exists(CnCFullGeneralProg)) {
		app = "\"" + CnCFullGeneralProg + "\"";
		ShellExecute(NULL, NULL, utf8ToUtf16(app).c_str(), NULL, NULL, SW_SHOW);
		Sleep(1000);
	}

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
			Sleep(3000);
			exit(0);
		}
		Sleep(1000);
	}
}

