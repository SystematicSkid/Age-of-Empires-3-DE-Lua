#pragma once
#include "../../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class ProtoUnit
	{

	public:
#define DEFINE_MEMBER_CLASS Engine::Addresses::ProtoUnit::Offsets
		union
		{
			DEFINE_MEMBER_B(INT, Id);
			DEFINE_MEMBER_B(PCHAR, Name);
		};
#undef DEFINE_MEMBER_CLASS
	};
}