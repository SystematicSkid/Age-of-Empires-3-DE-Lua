#pragma once
#include <Windows.h>

namespace Engine
{
	/* Game Modules */
	static DWORD64 ModuleGame = (DWORD64)GetModuleHandleA("AoE3DE_s.exe");

	namespace Addresses
	{

		namespace MultiplayerImpl
		{
			namespace Functions
			{
				 
			}
		}

		namespace Game
		{
			namespace Offsets
			{
				DWORD64 CONST World = 0x148;
				DWORD64 CONST ResourceManager = World + 0x8;
				DWORD64 CONST Camera = 0x158;
				DWORD64 CONST LocalPlayerId = 0x400;
			}

			namespace Functions
			{
				DWORD64 CONST SpawnUnits = Memory::GetCallAddress("E8 ? ? ? ? 48 8B 74 24 ? 48 8B 5C 24 ? 48 8B 6C 24 ? 48 8B 7C 24");
				DWORD64 CONST SetFogAndBlackMap = Memory::SigScan("40 53 48 83 EC 20 48 8B 05 ? ? ? ? 0F B6 DA 0F B6 D1");
				DWORD64 CONST GetLocalPlayer = Memory::GetCallAddress("E8 ? ? ? ? 48 39 07");
				DWORD64 CONST GetMaxSelectionCount = Memory::GetCallAddress("E8 ? ? ? ? 3B D8 7D 56");
				DWORD64 CONST GetMaxCardCount = Memory::GetCallAddress("E8 ? ? ? ? 39 86 ? ? ? ? 7E 1F");
			}
			DWORD64 CONST Instance = Memory::GetInstanceAddress("48 8B 0D ? ? ? ? 80 3D ? ? ? ? ? 74 54");
		}

		namespace World
		{
			namespace Offsets
			{
				DWORD64 CONST Objects = 0x18;
				DWORD64 CONST NumObjects = Objects + 0x8;
				DWORD64 CONST MaxObjects = NumObjects + 0x4;
				DWORD64 CONST Players = 0x98;
				DWORD64 CONST NumPlayers = Players + 0x8;
				DWORD64 CONST MaxPlayers = NumPlayers + 0x4;
				DWORD64 CONST MapName = 0xB0;
			}

			namespace Functions
			{
				DWORD64 CONST OnUpdate = Memory::SigScan("48 89 5C 24 ? 57 48 83 EC 20 0F B6 B9");
			}
		}

		namespace Unit
		{

			namespace Offsets
			{
				DWORD64 CONST Id = 0x20;
				DWORD64 CONST Hitpoints_Encrypted = 0xC8;
			}

			namespace Functions
			{
				DWORD64 CONST GetPosition = Memory::GetCallAddress("E8 ? ? ? ? 0F 10 38");
				DWORD64 CONST GetPrototype = Memory::GetCallAddress("E8 ? ? ? ? 3B 04 2F"); 
				DWORD64 CONST GetOwner = Memory::GetCallAddress("E8 ? ? ? ? 39 45 38"); 
			}
		}

		namespace ProtoUnit
		{
			namespace Offsets
			{
				DWORD64 CONST Id = 0x10;
				DWORD64 CONST Name = 0x18;
			}
		}

		namespace Renderer
		{
			namespace Offsets
			{
				DWORD64 CONST ViewProjectionMatrix = 0x240;
				DWORD64 CONST Context = 0x4C48;
				DWORD64 CONST NearClip = 0x7C80;
				DWORD64 CONST CameraX = 0x7C88;
				DWORD64 CONST CameraY = 0x7C8C;
				DWORD64 CONST Width = 0x7CAC;
				DWORD64 CONST Height = 0x7CB0;
			}

			namespace Functions
			{
				DWORD64 CONST MatrixVectorMultiply = Memory::GetCallAddress("E8 ? ? ? ? 4C 8D 45 4C");
			}
			// w2s: E8 ? ? ? ? 84 C0 75 24 48 83 3B 00 
			DWORD64 CONST Instance = Memory::GetInstanceAddress("48 8B 0D ? ? ? ? 48 81 C1 ? ? ? ? E8 ? ? ? ? E8 ? ? ? ? E8");
		}

		namespace Player
		{
			namespace Offsets
			{
				DWORD64 CONST Name = 0x8;
				DWORD64 CONST Age = 0x80;
				DWORD64 CONST Encrypted_Population = 0xC0;
				DWORD64 CONST Encrypted_PopulationCap = 0xD0;
				DWORD64 CONST HomeCity = 0xF8;
				DWORD64 CONST ResourceList = 0x328 + 0x10;
			}

			namespace Functions
			{
				DWORD64 CONST GetHomeCityLevel = Memory::GetCallAddress("E8 ? ? ? ? 3B C6 7C 04"); 
				DWORD64 CONST GetPopulationCount = Memory::GetCallAddress("E8 ? ? ? ? 8D 1C 07"); 
				DWORD64 CONST SetResourceAmount = Memory::GetCallAddress("E8 ? ? ? ? B0 01 EB 0F");
			}
		}

		namespace ResourceManager
		{
			namespace Offsets
			{
				DWORD64 CONST Prototypes = 0x80;
				DWORD64 CONST NumPrototypes = Prototypes + 0x8;
				DWORD64 CONST MaxPrototypes = NumPrototypes + 0x4;

			}
		}

		namespace Syscall
		{
			namespace Offsets
			{
				DWORD64 CONST Callback		= 0x0008;
				DWORD64 CONST Description	= 0x0010;
			}

			namespace Functions
			{
				DWORD64 CONST SetupXsCallbacks = Memory::GetCallAddress("E8 ? ? ? ? B0 01 48 83 C4 28 C3 48 89 5C 24 ? 48 89 6C 24");
			}
		}

		namespace SyscallModule
		{
			namespace Offsets
			{
				DWORD64 CONST Syscalls		= 0x0010;
				DWORD64 CONST NumSyscalls	= 0x0008 + Syscalls;
				DWORD64 CONST MaxSyscalls	= 0x0004 + NumSyscalls;
			}
		}

		namespace Console
		{
			namespace Offsets
			{
				DWORD64 CONST SyscallModule = 0x0090;
			}
			DWORD64 CONST Instance = Memory::GetInstanceAddress("48 8B 0D ? ? ? ? 41 B9 ? ? ? ? E8 ? ? ? ? 8B 15 ? ? ? ? 4C 8D 05 ? ? ? ? 48 8B 0D ? ? ? ? 41 B9 ? ? ? ? E8 ? ? ? ? 48 8B CB");
		}

		namespace XMLReader
		{
			namespace Functions
			{
				DWORD64 CONST LoadFile = Memory::GetCallAddress("E8 ? ? ? ? 84 C0 74 0A 4C 8B 6C 24");
			}
		}
	}
}