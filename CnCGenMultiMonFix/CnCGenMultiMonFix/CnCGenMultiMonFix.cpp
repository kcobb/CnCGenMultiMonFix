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
#include <vector>
#include <shellapi.h>
#include <cwctype>
#include "resource.h"

using namespace std;

//
// String trimming utilities
//
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

inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

inline void ltrim(std::wstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
		return !std::iswspace(ch);
	}));
}

inline void rtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
		return !std::iswspace(ch);
	}).base(), s.end());
}

inline void trim(std::wstring &s) {
	ltrim(s);
	rtrim(s);
}

//
// File existence check
//
inline bool exists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

//
// UTF conversion utilities
//
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

//
// Reads a string value from the registry, supports REG_SZ and REG_EXPAND_SZ
//
std::wstring ReadRegistryValue(HKEY hKey, const wchar_t* subKey, const wchar_t* valueName) {
	HKEY key;
	std::wstring result;

	// Open the registry key
	if (RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &key) == ERROR_SUCCESS) {
		DWORD dataSize = 0;
		DWORD dataType = 0;

		// Query the size and type of the registry value
		if (RegQueryValueEx(key, valueName, nullptr, &dataType, nullptr, &dataSize) == ERROR_SUCCESS && dataSize > 0) {
			if (dataType == REG_SZ || dataType == REG_EXPAND_SZ) {
				// Allocate buffer for the string value, including space for null terminator
				std::wstring buffer(dataSize / sizeof(wchar_t), L'\0');

				// Read the string value into the buffer
				if (RegQueryValueEx(key, valueName, nullptr, nullptr, reinterpret_cast<LPBYTE>(&buffer[0]), &dataSize) == ERROR_SUCCESS) {
					// Calculate the number of wchar_t characters read (including null terminator)
					size_t charCount = dataSize / sizeof(wchar_t);

					// If last character is null terminator, remove it from the length
					if (charCount > 0 && buffer[charCount - 1] == L'\0') {
						charCount--;
					}

					buffer.resize(charCount);

					// For REG_EXPAND_SZ, expand environment variables
					if (dataType == REG_EXPAND_SZ) {
						DWORD expandedSize = ExpandEnvironmentStrings(buffer.c_str(), nullptr, 0);
						if (expandedSize > 0) {
							std::wstring expandedBuffer(expandedSize, L'\0');
							ExpandEnvironmentStrings(buffer.c_str(), &expandedBuffer[0], expandedSize);
							// Remove the null terminator added by ExpandEnvironmentStrings
							if (!expandedBuffer.empty() && expandedBuffer.back() == L'\0') {
								expandedBuffer.pop_back();
							}
							result = expandedBuffer;
						}
						else {
							// Fallback if expansion fails
							result = buffer;
						}
					}
					else {
						result = buffer;
					}
				}
			}
		}
		RegCloseKey(key);
	}

	return result;
}

//
// Finds the first visible window matching class and/or title.
// Use nullptr for class if you want to ignore class matching.
//
HWND FindVisibleWindow(LPCWSTR lpClass, LPCWSTR lpTitle)
{
	HWND hwndNext = nullptr;
	HWND hwnd = nullptr;

	do
	{
		hwnd = FindWindowEx(nullptr, hwndNext, lpClass, lpTitle);
		if (hwnd && IsWindowVisible(hwnd))
			break;

		hwndNext = hwnd;
	} while (hwnd != nullptr);

	return hwnd;
}

//
// Minimize console window to tray with icon and tooltip
//
void MinimizeToTray(HWND hwndConsole)
{
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwndConsole;

	nid.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1));

	nid.uFlags = NIF_ICON | NIF_TIP;
	wcscpy_s(nid.szTip, L"C&C Lock Running");

	Shell_NotifyIcon(NIM_ADD, &nid);

	ShowWindow(hwndConsole, SW_HIDE);
}

//
// Remove tray icon
//
void RemoveTrayIcon(HWND hwndConsole)
{
	NOTIFYICONDATA nid = { 0 };
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hwndConsole;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

int main(int argc, char* argv[])
{
	const char spinner[] = { '|', '/', '-', '\\' };
	std::string CnCFullGeneralProg;
	std::string app;
	HWND hwndConsole = GetConsoleWindow();

	const std::vector<std::wstring> wcWinNameText = {
		L"Command & Conquer Generals",
		L"Command & Conquer (TM) Generals Zero Hour"
	};

	SetConsoleTitleA("C&C Generals - MultiMonitor Lock");

	HWND window = nullptr;
	RECT r;
	bool bWinLock = false;

	// Registry read (same as before)
	std::wstring RegInstallPath = ReadRegistryValue(
		HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\WOW6432Node\\Electronic Arts\\EA Games\\Command and Conquer Generals Zero Hour",
		L"InstallPath");

	if (RegInstallPath.empty()) {
		RegInstallPath = ReadRegistryValue(
			HKEY_LOCAL_MACHINE,
			L"SOFTWARE\\Electronic Arts\\EA Games\\Command and Conquer Generals Zero Hour",
			L"InstallPath");
	}

	if (!RegInstallPath.empty()) {
		rtrim(RegInstallPath);
		if (!RegInstallPath.empty() && RegInstallPath.back() == L'\\') {
			RegInstallPath.pop_back();
		}
		CnCFullGeneralProg = utf16ToUtf8(RegInstallPath) + "\\generals.exe";
	}

	if (exists(CnCFullGeneralProg)) {
		std::cout << "Launching: " << CnCFullGeneralProg << std::endl;
		HINSTANCE res = ShellExecute(nullptr, nullptr, utf8ToUtf16(CnCFullGeneralProg).c_str(), nullptr, nullptr, SW_SHOW);
		if ((INT_PTR)res <= 32) {
			std::cerr << "Failed to launch application, error code: " << (INT_PTR)res << std::endl;
		}
		Sleep(1000);
	}

	int intLoopCount = 0;

	while (true) {
		intLoopCount++;
		std::cout << "\rSearching for C&C game window... " << std::flush;

		// Only set window to nullptr here (don't reset bWinLock prematurely)
		window = nullptr;

		for (const auto& title : wcWinNameText) {
			window = FindVisibleWindow(nullptr, const_cast<wchar_t*>(title.c_str()));
			if (window != nullptr) {
				bWinLock = true;
				break;
			}
		}

		if (bWinLock && window != nullptr) {
			std::cout << "\rGame window locked " << spinner[intLoopCount % 4] << "     " << std::flush;
			GetWindowRect(window, &r);
			ClipCursor(&r);

			static bool minimized = false;
			if (!minimized) {
				MinimizeToTray(hwndConsole);
				minimized = true;
			}
		}

		if (bWinLock && window == nullptr) {
			std::cout << "\rGame window lost focus, closing app     " << std::flush;
			ClipCursor(nullptr);
			RemoveTrayIcon(hwndConsole);
			Sleep(3000);
			return 0;
		}

		Sleep(1000);
	}
}

