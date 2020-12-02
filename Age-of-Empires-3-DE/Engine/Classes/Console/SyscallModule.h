#pragma once
#include "../../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class SyscallModule
	{

	public:
#define DEFINE_MEMBER_CLASS Engine::Addresses::SyscallModule::Offsets
		union
		{
			DEFINE_MEMBER_B(Engine::Syscall**, Syscalls);
			DEFINE_MEMBER_B(INT, NumSyscalls);
			DEFINE_MEMBER_B(INT, MaxSyscalls);
		};
#undef DEFINE_MEMBER_CLASS
	};
}