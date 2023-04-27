#include "pch.h"
#include "HUtil.h"
#include "HTools.h"
#include <iostream>
#include <Windows.h>

using namespace std;

HUtil::HUtil(LPCWSTR module) : m_ConsoleOut(NULL), m_ConsoleOutBackup(NULL) {
	m_Base = (DWORD)GetModuleHandle(module);
}

HUtil::~HUtil() {}

DWORD HUtil::Offset(DWORD offset) {
	return m_Base + offset;
}

void HUtil::OpenConsole(const char* name) {
	if (!m_ConsoleOpen) {
		if (AllocConsole()) {
			m_ConsoleOutBackup = cout.rdbuf();
			m_ConsoleOut = freopen("CONOUT$", "w", stdout);
			char nt[100];
			sprintf_s(nt, "%s-%lld-%d", name, GetTickCount64(), GetCurrentProcessId());
			SetConsoleTitleA(nt);
			Sleep(100);
			HWND find = FindWindowA(NULL, nt);
			if (find) {
				HMENU menu = GetSystemMenu(find, FALSE);
				if (menu) {
					if (RemoveMenu(menu, 0xF060, 0)) {
						sprintf_s(nt, "%s-%lld-%d-ÒÑÆÁ±Î¹Ø±Õ°´Å¥", name, GetTickCount64(), GetCurrentProcessId());
						SetConsoleTitleA(nt);
					}
				}
			}
		}
		wcout.imbue(locale("", LC_CTYPE));
		m_ConsoleOpen = TRUE;
	}
}

void HUtil::CloseConsole() {
	if (m_ConsoleOpen) {
		if (m_ConsoleOut != NULL && m_ConsoleOutBackup != NULL) {
			cout.rdbuf(m_ConsoleOutBackup);
			fclose(m_ConsoleOut);
		}
		FreeConsole();
		m_ConsoleOpen = FALSE;
	}
}

void HUtil::LoadHook(DWORD runAddress, DWORD hookAddress, LPVOID backup) {
	BYTE jmpCode[5] = { 0 };
	jmpCode[0] = 0xE9;
	*(DWORD*)&jmpCode[1] = runAddress - hookAddress - 5;
	ReadProcessMemory(GetCurrentProcess(), (LPVOID)hookAddress, backup, 5, 0);
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookAddress, jmpCode, 5, 0);
}

void HUtil::UnloadHook(DWORD hookAddress, LPVOID backup) {
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)hookAddress, backup, 5, 0);
}