#include "pch.h"
#include "Hell.h"

using namespace std;

HMODULE g_Ntdll = GetModuleHandleW(L"ntdll.DLL");
HMODULE g_HModule = NULL;
void* g_PvCookie = NULL;

//HOOK的模块名
LPCWSTR g_Module = L"MapleStory.exe";
//Console标题
const char* g_Name = "地狱";
HUtil* Util = new HUtil(g_Module);

void InHook() {

}

void UnHook() {

}

VOID NTAPI LdrDllNotification(ULONG NotificationReason, PCLDR_DLL_NOTIFICATION_DATA NotificationData, PVOID Context) {
    if (NotificationReason == LDR_DLL_NOTIFICATION_REASON_LOADED) {
        if (wcscmp(NotificationData->Loaded.BaseDllName->Buffer, g_Module) == 0) {
            
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    PfnLdrRegisterDllNotification PLdrRegisterDllNotification;
    PfnLdrUnregisterDllNotification PLdrUnregisterDllNotification;
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        Util->OpenConsole(g_Name);
        g_HModule = hModule;
        if (g_Ntdll != NULL) {
            PLdrRegisterDllNotification = (PfnLdrRegisterDllNotification)GetProcAddress(g_Ntdll, "LdrRegisterDllNotification");
            PLdrRegisterDllNotification(0, LdrDllNotification, NULL, &g_PvCookie);
        }
        InHook();
        break;
    case DLL_PROCESS_DETACH:
        UnHook();
        Util->CloseConsole();
        if (g_Ntdll != NULL) {
            PLdrUnregisterDllNotification = (PfnLdrUnregisterDllNotification)GetProcAddress(g_Ntdll, "LdrUnregisterDllNotification");
            PLdrUnregisterDllNotification(g_PvCookie);
        }
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

