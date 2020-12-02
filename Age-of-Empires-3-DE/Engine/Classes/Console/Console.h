#pragma once
#include "../../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class Console
	{
	public:
		static Console*& Instance;
	public:
#define DEFINE_MEMBER_CLASS Engine::Addresses::Console::Offsets
		union
		{
			DEFINE_MEMBER_B(Engine::SyscallModule*, SyscallModule);
		};
#undef DEFINE_MEMBER_CLASS
	};
	Engine::Console*& Engine::Console::Instance = *(Engine::Console**)(Engine::Addresses::Console::Instance);

}