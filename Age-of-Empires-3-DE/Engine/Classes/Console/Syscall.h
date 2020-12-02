#pragma once
#include "../../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class Syscall
	{

	public:
#define DEFINE_MEMBER_CLASS Engine::Addresses::Syscall::Offsets
		union
		{
			DEFINE_MEMBER(INT, Id);
			DEFINE_MEMBER_B(PVOID, Callback);
			DEFINE_MEMBER_B(PCHAR, Description);
		};
#undef DEFINE_MEMBER_CLASS
	public:
		std::string GetName()
		{
			std::string desc(Description);
			if (memcmp(Description, "void", 0x4) == 0 || memcmp(Description, "int", 0x3) == 0)
				desc = desc.substr(desc.find_first_of(' ')+2);
			
			INT first_non_alpha_idx = -1;
			for (int i = 0; i < desc.length() - 1; i++)
			{
				if (!isalpha(desc.c_str()[i]))
				{
					first_non_alpha_idx = i;
					break;
				}
			}

			return desc.substr(0, first_non_alpha_idx);
		}
	};
}