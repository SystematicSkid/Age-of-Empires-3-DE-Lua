#pragma once
#include "../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class World
	{

	public:
#define DEFINE_MEMBER_CLASS Engine::Addresses::World::Offsets
		union
		{
			DEFINE_MEMBER(void**, VMTable);
			DEFINE_MEMBER_B(Engine::Unit**, Objects);
			DEFINE_MEMBER_B(INT, NumObjects);
			DEFINE_MEMBER_B(INT, MaxObjects);
			DEFINE_MEMBER_B(Engine::Player**, Players);
			DEFINE_MEMBER_B(INT, NumPlayers);
			DEFINE_MEMBER_B(INT, MaxPlayers);
		};
#undef DEFINE_MEMBER_CLASS

	public:
		Engine::Unit* GetUnit(INT idx)
		{
			if (idx > this->NumObjects)
				return nullptr;
			return Objects[idx];
		}

		Engine::Player* GetPlayer(INT idx)
		{
			if (idx > this->NumPlayers)
				return nullptr;
			return Players[idx];
		}
	};
}