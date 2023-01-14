#include "includes.h"

#include <windows.h>
#include <vector>
#include <string>
#include <thread>
#include <stdio.h>
#include <lmcons.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <wchar.h>
#include <tchar.h>
#include <random>
#include <sstream>
#include <ShlObj.h>
#include <functional>
#include "font_awesome.cpp"


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	//io.Fonts->AddFontDefault();
	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 }; //ICON_MIN_FA , ICON_MAX_FA
	ImFontConfig icons_config;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 1.0;
	icons_config.OversampleV = 1.0;

	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.WantCaptureKeyboard;
	io.WantCaptureMouse;
	io.FontAllowUserScaling;

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 15);
	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 15.0f, &icons_config, icons_ranges);

	ImGui_ImplWin32_Init(window); //window
	ImGui_ImplDX11_Init(pDevice, pContext);

	int LogoWidth = 16;
	int LogoHeight = 16;
}

bool init = false;
static bool showui = true;
static bool HasSkin = true;
static bool HasEmote = true;
static bool HasAnimation = true;
static bool HasFootstep = true;

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (showui)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");
uintptr_t HasSkinOffset = 0x12F0DA0;
uintptr_t HasEmoteOffset = 0x12F0C00;
uintptr_t HasAnimationOffset = 0x12F0AD0;
uintptr_t HasFootstepOffset = 0x12F0D10;

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

void feature()
{
	if (HasSkin)
	{
		// Skin
		MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasSkinOffset), &HasSkin_h, (LPVOID*)&has_skin_o);
		MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasSkinOffset));
	}
	else
	{
		MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasSkinOffset));
	}

	if (HasEmote)
	{
		// Emote
		MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasEmoteOffset), &HasEmote_h, (LPVOID*)&has_emote_o);
		MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasEmoteOffset));
	}
	else
	{
		MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasEmoteOffset));
	}

	if (HasAnimation)
	{
		// Animation
		MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasAnimationOffset), &HasAnimation_h, (LPVOID*)&has_animation_o);
		MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasAnimationOffset));
	}
	else
	{
		MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasAnimationOffset));
	}

	if (HasFootstep)
	{
		// Footstep
		MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + HasFootstepOffset), &HasFootstep_h, (LPVOID*)&has_footstep_o);
		MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasFootstepOffset));
	}
	else
	{
		MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + HasFootstepOffset));
	}
}
static char buffer[9999999];
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}
		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	if (GetAsyncKeyState(VK_INSERT) & 1) {
		showui = !showui;
		return 0;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::GetMouseCursor();
	ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	ImGui::GetIO().WantCaptureMouse = showui;
	ImGui::GetIO().MouseDrawCursor = showui;

	if (showui)
	{
		if (ImGui::Begin("Stumble-Guys | Der Dom muss weg", &showui)) {
			ImGui::SetWindowSize(ImVec2(600, 450), ImGuiCond_Always);
			ImGui::Checkbox("Unlock Skins", &HasSkin);
			ImGui::Checkbox("Unlock Emotes", &HasEmote);
			ImGui::Checkbox("Unlock Animations", &HasAnimation);
			ImGui::Checkbox("Unlock Footsteps", &HasFootstep);
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
		switch (dwReason)
		{
		case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls(hMod);
			CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
			break;
		case DLL_PROCESS_DETACH:
			kiero::shutdown();
			break;
		}
		return TRUE;
}