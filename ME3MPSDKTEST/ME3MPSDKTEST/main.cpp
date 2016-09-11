﻿#include "header.h"

USFXGUI_MPHUD *mphud;
USFXOnlineComponentOrigin *origincomp;
Asfxgrimp *grimp;
Asfxgrimp_lobby *grimp_lobby;
USFXWave_Horde *sfxwavehorde;
wchar_t presencetext[256];
wchar_t textmap[31][17] = { L"Unknown", L"map1", L"Dagger", L"Ghost", L"Giant",
							L"Reactor", L"map6", L"Glacier", L"White", L"Condor",
							L"Hydra", L"Jade", L"Gauntlet", L"Goddess", L"Rio",
							L"Vancouver", L"London", L"☣Glacier☣", L"☣Dagger☣", L"☣Reactor☣",
							L"☣Ghost☣", L"☣Giant☣", L"☣White☣", L"Original White", L"Original Glacier", L"Scorpio", L"Neptune", L"Forge", L"Maelstrom", L"Dragon", L"Athena" };
wchar_t textenemy[8][18] = { L"Unknown", L"Cerberus", L"Geth", L"Reaper", L"Collector", L"Original Cerberus", L"Original Geth", L"Original Reaper" };
wchar_t textdif[4][16] = { L"Bronze", L"Silver", L"Gold", L"Platinum" };
DWORD lobbyPointer;

bool checkGameHasFocus()
{
	HWND fgWindow = GetForegroundWindow();
	DWORD fgPID;
	if (GetWindowThreadProcessId(fgWindow, &fgPID) == 0)
		return false;
	return (fgPID == GetCurrentProcessId());
}

DWORD WINAPI RichPresenceUpdater(LPVOID lpParam)
{
	Sleep(5000);
	do
	{
		Sleep(10000);
		if (!checkGameHasFocus())
			continue;
		origincomp = (USFXOnlineComponentOrigin*)UObject::FindObject<UObject>("SFXOnlineComponentOrigin Transient.SFXOnlineComponentOrigin");
		grimp = (Asfxgrimp*)UObject::FindObject<UObject>("sfxgrimp TheWorld.PersistentLevel.sfxgrimp");
		grimp_lobby = (Asfxgrimp_lobby*)UObject::FindObject<UObject>("sfxgrimp_lobby TheWorld.PersistentLevel.sfxgrimp_lobby");
		if (origincomp && grimp_lobby)
		{
			if (*(int*)lobbyPointer != 0)
				wsprintfW(presencetext, L"MP Lobby: %ls/%ls/%ls", textmap[grimp_lobby->MapSetting], textenemy[grimp_lobby->EnemySetting], textdif[grimp_lobby->DifficultySetting]);
			else {
				wsprintfW(presencetext, L"MP");
			}
			origincomp->SetRichPresence(presencetext, L"");

		}
		else if (origincomp && grimp)
		{
			wsprintfW(presencetext, L"MP Match: %ls/%ls/%ls", textmap[grimp->MapSetting], textenemy[grimp->EnemySetting], textdif[grimp->DifficultySetting]);
			origincomp->SetRichPresence(presencetext, L"");
		}
		else if (origincomp) 
		{
			origincomp->SetRichPresence(L"SP", L"");
		}
	} while (true);
	return 0;
}


bool __stdcall DllMain(HANDLE process, DWORD reason, LPVOID lpReserved) {
	if (reason == DLL_PROCESS_ATTACH)
	{
		lobbyPointer = (DWORD)GetModuleHandle(NULL) + 0x152698C;
		CreateThread(0, 0, &RichPresenceUpdater, 0, 0, 0);
		return 1;
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		return 1;
	}
	return 0;
}