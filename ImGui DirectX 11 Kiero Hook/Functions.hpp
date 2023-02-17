#pragma once
#include "includes.h"
#include "iostream"

namespace Offsets
{
	uintptr_t moduleBase = (uintptr_t)GetModuleHandle("GameAssembly.dll");
	uintptr_t HasSkinOffset = 0x3FA1A0;
	uintptr_t HasEmoteOffset = 0x3FA000;
	uintptr_t HasAnimationOffset = 0x3F9ED0;
	uintptr_t HasFootstepOffset = 0x3FA110;
}

bool(__fastcall* has_skin_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasSkin_h(DWORD* __this, const char* skinID, DWORD* method)
{
	return true;
}

bool(__fastcall* has_emote_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasEmote_h(DWORD* __this, const char* emoteID, DWORD* method)
{
	return true;
}

bool(__fastcall* has_animation_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasAnimation_h(DWORD* __this, const char* animationID, DWORD* method)
{
	return true;
}

bool(__fastcall* has_footstep_o)(DWORD*, const char*, DWORD*);
bool __stdcall HasFootstep_h(DWORD* __this, const char* footstepID, DWORD* method)
{	
	return true;
}

void UnlockAllSkins() {
	MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::moduleBase + Offsets::HasSkinOffset), &HasSkin_h, (LPVOID*)&has_skin_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(Offsets::moduleBase + Offsets::HasSkinOffset));
}

void UnlockAllEmotes() {
	MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::moduleBase + Offsets::HasEmoteOffset), &HasEmote_h, (LPVOID*)&has_emote_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(Offsets::moduleBase + Offsets::HasEmoteOffset));
}

void UnlockAllAnimations() {
	MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::moduleBase + Offsets::HasAnimationOffset), &HasAnimation_h, (LPVOID*)&has_animation_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(Offsets::moduleBase + Offsets::HasAnimationOffset));
}

void UnlockAllFootstep() {
	MH_CreateHook(reinterpret_cast<LPVOID*>(Offsets::moduleBase + Offsets::HasFootstepOffset), &HasFootstep_h, (LPVOID*)&has_footstep_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(Offsets::moduleBase + Offsets::HasFootstepOffset));
}