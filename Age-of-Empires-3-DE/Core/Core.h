#pragma once
#define WIN32_LEAN_AND_MEAN
// win
#include <Windows.h>
#include <winternl.h>
#include <Xinput.h>
#include <d3d11.h>
#include <d3d9.h>
#include <D3DX10math.h>
#include <comdef.h>
#include <minwindef.h>
#include <winbase.h>
#include <intrin.h>
#include <cor.h>
#include <cordebug.h>
#include <metahost.h>
#include <psapi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment (lib, "D3DCompiler.lib")
#pragma comment(lib, "DWrite.lib")
#pragma comment (lib, "d3dx9.lib")

// stl
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include <clocale>
#include <algorithm>
#include <sstream>
#include <functional>

using Vector2 = D3DXVECTOR2;
using Vector3 = D3DXVECTOR3;
using Vector4 = D3DXVECTOR4;

#define NOMINMAX
#define NOMCX
#define NOIME
#define NOSERVICE
#define NOKANJI

constexpr float M_PI = 3.14159265358979323846f;

template<class T>
T clamp(T val, T min, T max) {
	return std::max(min, std::min(max, val));
}

// Utility
#include "../Utility/Memory.h"


/* Engine */
#include "../Engine/Engine.h"

/* Lua */
#include "./Lua/Lua.h"
#include "./Lua/Binding.h"

/* Core Hooks */
#include "./Hooks/Hooks.h"

extern ID3D11DeviceContext* pContext;
extern ID3D11Device* pDevice;


/* Renderer */
#include "./Renderer/renderer.h"

bool first = true;

namespace Core
{

	font text;

	std::once_flag init_flag;

	BOOL Initialize(VOID)
	{
		printf("[ + ] Initializing Core...\n");

		printf("\t [ + ] Hooking functions\n");
		Core::Hooks::Initialize();
		printf("\t [ + ] Initializing Lua interface...\n");
		Core::Lua::Binding::Initialize();


		Core::Hooks::DirectX::OnPresent = [=](IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
		{
			std::call_once(init_flag, [=]()
				{
					printf("[ + ] Initializing renderer...\n");
					pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice);
					pDevice->GetImmediateContext(&pContext);
					renderer_setup(pDevice, pContext);

					text.create("Tahoma", 8, FONT_DEFAULT);
				});

			if (GetAsyncKeyState(VK_F11) & 0x8000)
			{
				// Rebuild Lua
				Lua::Binding::Initialize();
				return;
			}

			Lua::Binding::HookCall("OnUpdate", Engine::Game::Instance);

			renderer_start_drawing(); 
			geometry g;
			g.push_draw_cmd();

			text.draw_text(&g, { 60, 30 }, "AoE3DE - Engine", { 255,255,255,255 }, TEXT_CENTERED | TEXT_OUTLINE ,{ 0,0,0,127 });
			text.draw_text(&g, { 60, 50 }, "F11 - Reload Lua", { 255,255,255,255 }, TEXT_CENTERED | TEXT_OUTLINE, { 0,0,0,127 });


			auto world = Engine::Game::Instance->World;
			if (world)
			{
				/*if (Engine::Game::Instance->GetLocalPlayer())
				{
					Engine::Game::Instance->GetLocalPlayer()->SetResource(Engine::Resource::Food, 100000.f);
					Engine::Game::Instance->GetLocalPlayer()->SetResource(Engine::Resource::Gold, 100000.f);
					Engine::Game::Instance->GetLocalPlayer()->SetResource(Engine::Resource::Wood, 100000.f);
				}*/

				/*
					W2S Testing
				*/

				/*for (int i = 0; i < world->NumObjects; i++)
				{
					auto unit = world->Objects[i];
					if (unit)
					{
						if (unit->GetOwner()->Id == 0)
							continue;
						D3DXVECTOR2 screen;
						if (unit->WorldToScreen(&screen))
						{
							auto prototype = unit->GetPrototype();
							text.draw_text(&g, { screen.x, screen.y }, prototype->Name, { 255,255,255,255 }, TEXT_CENTERED | TEXT_OUTLINE, { 0,0,0,127 });
						}
					}
				}*/
			}

			g.render();
			renderer_draw();
		};

		return TRUE;
	}
}
