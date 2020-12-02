#pragma once
#include "../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class Renderer
	{
	public:
		/* Instance */
		static Renderer*& Instance;
	public:
#define DEFINE_MEMBER_CLASS Engine::Addresses::Renderer::Offsets
		union
		{
			DEFINE_MEMBER_B(D3DXMATRIX, ViewProjectionMatrix);
			DEFINE_MEMBER_B(PVOID, Context);

			DEFINE_MEMBER_B(FLOAT, NearClip);
			DEFINE_MEMBER_B(FLOAT, CameraX);
			DEFINE_MEMBER_B(FLOAT, CameraY);

			DEFINE_MEMBER_B(INT, Width);
			DEFINE_MEMBER_B(INT, Height);

		};
#undef DEFINE_MEMBER_CLASS
	public:
		VOID MatrixVectorMultiply(D3DXVECTOR3* vector3, D3DXVECTOR3* out)
		{
			return static_cast<VOID(__fastcall*)(DWORD64, D3DXVECTOR3*, D3DXVECTOR3*)>((PVOID)Engine::Addresses::Renderer::Functions::MatrixVectorMultiply)((DWORD64)this + 0x240, vector3, out);
		}
	};
	Engine::Renderer*& Engine::Renderer::Instance = *(Engine::Renderer**)(Engine::Addresses::Renderer::Instance);
}