#include <Windows.h>
#include "pch.h"
#include <Minhook.h>
#include <cstdio>

uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");
uintptr_t HasSkinOffset = 0x12F0DA0;
uintptr_t HasEmote = 0x12F0C00;
uintptr_t HasAnimation = 0x12F0AD0;
uintptr_t HasFootstep = 0x12F0D10;

void CreateConsole()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
}

void init()
{
    MH_Initialize();
    CreateConsole();
    //MessageBox(NULL, "Kilian", "Kilian", NULL);
}

bool(__fastcall* has_skin_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasSkin_h(DWORD* __this, const char* skinID, DWORD* method)
{
    printf("HasSkin called with skinID: %s", skinID + 0xA);
    return true;
}

bool(__fastcall* has_emote_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasEmote_h(DWORD* __this, const char* emoteID, DWORD* method)
{
    printf("HasEmote called with emoteID: %s", emoteID + 0xA);
    return true;
}

bool(__fastcall* has_animation_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasAnimation_h(DWORD* __this, const char* animationID, DWORD* method)
{
    printf("HasAnimation called with emoteID: %s", animationID + 0xA);
    return true;
}

bool(__fastcall* has_footstep_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasFootstep_h(DWORD* __this, const char* footstepID, DWORD* method)
{
    printf("HasFootstep called with emoteID: %s", footstepID + 0xA);
    return true;
}

void main()
{
    init();
    // Skin
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasSkinOffset), &HasSkin_h, (LPVOID*)&has_skin_o);
    MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasSkinOffset));
    // Emote
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasEmote), &HasEmote_h, (LPVOID*)&has_emote_o);
    MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasEmote));
    // Animation
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasAnimation), &HasAnimation_h, (LPVOID*)&has_animation_o);
    MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasAnimation));
    // Footstep
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasFootstep), &HasFootstep_h, (LPVOID*)&has_footstep_o);
    MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasFootstep));
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

