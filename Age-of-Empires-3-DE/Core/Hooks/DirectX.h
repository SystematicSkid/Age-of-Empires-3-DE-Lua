#pragma once

namespace Core
{
	namespace Hooks
	{
		namespace DirectX
		{
			/* Our Callback */
			std::function<VOID(IDXGISwapChain * pSwapChain, UINT SyncInterval, UINT Flags)> OnPresent = nullptr;

			using D3D11PresentHook = HRESULT(__stdcall*) (IDXGISwapChain * pSwapChain, UINT SyncInterval, UINT Flags);
			D3D11PresentHook phookD3D11Present = NULL;

			/* Address of original functions */
			PVOID OriginalFunction = NULL;

			/* Raw callback of `DirectX->EndScene` */
			HRESULT __stdcall NewPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
			{
				//auto protection_object = present_hook->GetProtectionObject();
				// Call event `Hooks::DirectX->OnEndScene`
				if (Hooks::DirectX::OnPresent)
				{
					Hooks::DirectX::OnPresent(pSwapChain, SyncInterval, Flags);
				}

				// Return original function
				return static_cast<HRESULT(__stdcall*)(IDXGISwapChain*, UINT, UINT)>((PVOID)OriginalFunction)(pSwapChain, SyncInterval, Flags);
			}

			void Initialize()
			{
				auto wnd = (HWND)FindWindowA("Age3DE", NULL);
				auto pSwapChain = Engine::Renderer::Instance->Context;
				DWORD64 myFunc = (DWORD64)NewPresent;
				DWORD64* engineInterface = (DWORD64*)*(DWORD64*)pSwapChain;

				if (engineInterface[8] != myFunc)
				{
					//Allocate some memory for our copy of the interface (vtable)
					static DWORD64 myEngineInterface[208 + 150];

					DWORD64* engineInterface = (DWORD64*)*(DWORD64*)pSwapChain;

					//Copy from the original to ours...
					memcpy(&myEngineInterface, engineInterface, sizeof(myEngineInterface));
					*(DWORD64*)pSwapChain = (DWORD64)&myEngineInterface;
					//Now apply the hook to our interface.
					OriginalFunction = (PVOID)myEngineInterface[8];
					myEngineInterface[8] = (DWORD64)myFunc;
				}
			}
		}
	}
}