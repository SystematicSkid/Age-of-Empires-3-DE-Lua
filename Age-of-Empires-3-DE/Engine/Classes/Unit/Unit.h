#pragma once
#include "../../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class Unit
	{

	public:
		#define DEFINE_MEMBER_CLASS Engine::Addresses::Unit::Offsets
		union
		{
			DEFINE_MEMBER_B(INT, Id);
			DEFINE_MEMBER_B(DWORD, Hitpoints_Encrypted);
		};
		#undef DEFINE_MEMBER_CLASS
	public:

		D3DXVECTOR3* GetPosition()
		{
			return static_cast<D3DXVECTOR3*(__fastcall*)(Unit*)>((void*)Engine::Addresses::Unit::Functions::GetPosition)(this);
		}

		Engine::ProtoUnit* GetPrototype()
		{
			DWORD64 key = 0x2B1D93A64E9F7D19;
			DWORD64 encrypted_val = *(DWORD64*)((uintptr_t)this + 0x98);
			return (Engine::ProtoUnit*)(encrypted_val - key);
		}

		Engine::Player* GetOwner()
		{
			DWORD64 key1 = 0x6F3D2BBB3A6249B2;
			DWORD64 key2 = 0x850C81B95CECBEE3;
			DWORD64 encrypted_val = *(DWORD64*)((uintptr_t)this + 0x48);

			return (Engine::Player*)((encrypted_val - key1) ^ key2);
		}

		BOOL WorldToScreen(D3DXVECTOR2* out)
		{
			auto renderer = Engine::Renderer::Instance;
			auto WorldPos = this->GetPosition();
			D3DXVECTOR3 res;
			renderer->MatrixVectorMultiply(this->GetPosition(), &res);
			if (renderer->NearClip > res.z)
				return FALSE;
			out->x = (float)((float)(res.x * renderer->CameraX) * (float)(1.0 / res.z)) + (float)((float)renderer->Width * 0.5);
			out->y = (float)((float)renderer->Height * 0.5) - (float)((float)(res.y * renderer->CameraY) * (float)(1.0 / res.z));
			return TRUE;
		}

		FLOAT GetHitpoints()
		{
			return *(FLOAT*)(this->Hitpoints_Encrypted - 0x1E33BF87);
		}
	};
}