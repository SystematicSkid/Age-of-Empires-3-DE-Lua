#pragma once
#include "PolyHook.hpp"

namespace Core
{
	namespace Hooks
	{
		namespace Game
		{
			PLH::VEHHook* heartbeat_hook = nullptr;
			PVOID original_heartbeat = nullptr;

			BOOL __fastcall NewSendHeartbeat(DWORD64 rcx, PCHAR value_type, PCHAR value, PCHAR unknown1, CHAR unknown2)
			{
				auto protection_object = heartbeat_hook->GetProtectionObject();
				printf("[ - ] Heartbeat\n");
				printf("\tType: %s\n", value_type);
				printf("\tValue: %s\n", value);
				printf("\tUnknown1: %s\n", unknown1);
				printf("\tUnknown2: %c\n", unknown2);
				return static_cast<BOOL(__fastcall*)(DWORD64, PCHAR, PCHAR, PCHAR, CHAR)>(original_heartbeat)(rcx, value_type, value, unknown1, unknown2);
			}

			PLH::VEHHook* checksum_hook = nullptr;
			PVOID original_checksum = nullptr;

			BOOL __fastcall NewWriteChecksum(PVOID thisptr, PVOID rdx, unsigned int size)
			{
				auto protection_object = checksum_hook->GetProtectionObject();
				auto ret = static_cast<BOOL(__fastcall*)(PVOID, PVOID, UINT)>(original_checksum)(thisptr, rdx, size);

				struct Decoder
				{
					PVOID* vtable;
					uint8_t* byte_array;
					PCHAR checksum;
					char pad_0018[288]; //0x0018
					wchar_t* file_name;
				}*decoder;
				decoder = (Decoder*)thisptr;
				printf("[ - ] Hashing file...\n");
				printf("\tChecksum: %s\n", decoder->checksum);
				strcpy(decoder->checksum, "A7NKF5YYOlHFHeMXNlKLmBlh4S9KpG3SPAXkAYJbNSA=");
				return ret;
			}

			PLH::VEHHook* selectioncount_hook = nullptr;
			PVOID original_selectioncount = nullptr;

			INT __stdcall NewGetMaxSelectionCount()
			{
				auto protection_object = selectioncount_hook->GetProtectionObject();
				return 90;
			}

			PLH::VEHHook* maxcard_hook = nullptr;
			PVOID original_maxcard = nullptr;

			INT __stdcall NewGetMaxDeckCards()
			{
				auto protection_object = maxcard_hook->GetProtectionObject();
				return 50;
			}

			PLH::VEHHook* callback_hook = nullptr;
			PVOID original_callback = nullptr;
			BOOL __fastcall SetupXsCallbacks(PVOID thisptr, PVOID a2, PVOID a3, PVOID a4)
			{
				printf("Initializing callbacks!\n");
				printf("thisptr: 0x%p\n", thisptr);
				printf("a2: 0x%p\n", a2);
				printf("a3: 0x%p\n", a3);
				printf("a4: 0x%p\n", a4);

				return static_cast<BOOL(__fastcall*)(PVOID, PVOID, PVOID, PVOID)>(original_callback)(thisptr, a2, a3, a4);
			}

			BOOL Initialize()
			{
				/*heartbeat_hook = new PLH::VEHHook();
				auto heartbeat_address = Memory::GetCallAddress("E8 ? ? ? ? 41 39 5F 68");
				heartbeat_hook->SetupHook((BYTE*)heartbeat_address, (BYTE*)&NewSendHeartbeat, PLH::VEHHook::VEHMethod::INT3_BP);
				if (!heartbeat_hook->Hook())
					return FALSE;
				original_heartbeat = heartbeat_hook->GetOriginal<PVOID>();*/


				selectioncount_hook = new PLH::VEHHook();
				selectioncount_hook->SetupHook((BYTE*)Engine::Addresses::Game::Functions::GetMaxSelectionCount, (BYTE*)&NewGetMaxSelectionCount, PLH::VEHHook::VEHMethod::INT3_BP);
				if (!selectioncount_hook->Hook())
					return FALSE;

				/*callback_hook = new PLH::VEHHook();
				callback_hook->SetupHook((BYTE*)Engine::Addresses::Syscalls::Functions::SetupXsCallbacks, (BYTE*)&SetupXsCallbacks, PLH::VEHHook::VEHMethod::INT3_BP);
				if (!callback_hook->Hook())
					return FALSE;
				original_callback = callback_hook->GetOriginal<PVOID>();*/

				return TRUE;
			}
		}
	}
}