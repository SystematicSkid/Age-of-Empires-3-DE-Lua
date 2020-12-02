#pragma once
#include "../../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class Player
	{

	public:
		#define DEFINE_MEMBER_CLASS Engine::Addresses::Player::Offsets
		union
		{
			DEFINE_MEMBER(INT, Id);
			DEFINE_MEMBER_B(INT, Age);
			DEFINE_MEMBER_B(DWORD64, Encrypted_Population);
			DEFINE_MEMBER_B(DWORD64, Encrypted_PopulationCap);
			DEFINE_MEMBER_B(DWORD*, ResourceList);
		};
		#undef DEFINE_MEMBER_CLASS
	public:
		INT GetPopulationCount()
		{
			//return this->Encrypted_Population + 0x7ADB68E696D199D0;
			return static_cast<INT(__fastcall*)(Engine::Player*)>((PVOID)Engine::Addresses::Player::Functions::GetPopulationCount)(this);
		}

		INT GetPopulationCap()
		{
			// Size might not overflow like population count, force it into a signed int
			return *(signed int*)(this->Encrypted_PopulationCap - 0x2D115B98B080CEB8);
		}

		FLOAT GetResourceAmount(Engine::Resource resource)
		{
			auto encrypted_val = (*(DWORD*)((DWORD64)this->ResourceList + 8 * (int)resource)) + 0x7BA9CCB8;
			auto decrypted_float = (PVOID)(encrypted_val ^ 0x86A4DFC9);
			return *(FLOAT*)&decrypted_float;
		}

		VOID SetResource(Engine::Resource resource, FLOAT value)
		{
			static_cast<DWORD64(__fastcall*)(DWORD64, Engine::Resource, FLOAT)>((PVOID)Engine::Addresses::Player::Functions::SetResourceAmount)((DWORD64)this + 0x328, resource, value);
		}

		//
	};
}