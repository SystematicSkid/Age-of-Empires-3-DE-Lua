#pragma once
#include "../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class ResourceManager
	{

	public:
#define DEFINE_MEMBER_CLASS Engine::Addresses::ResourceManager::Offsets
		union
		{
			DEFINE_MEMBER_B(Engine::ProtoUnit**, Prototypes);
			DEFINE_MEMBER_B(INT, NumPrototypes);
			DEFINE_MEMBER_B(INT, MaxPrototypes);
		};
#undef DEFINE_MEMBER_CLASS
	public:
		Engine::ProtoUnit* GetPrototype(INT idx)
		{
			if (idx > this->NumPrototypes)
				return nullptr;
			return Prototypes[idx];
		}
	};
}