#pragma once
#include "../Engine.h"

namespace Engine
{
#pragma  pack(push, 1)
	class Game
	{
	private:
		/* Context */
		static DWORD OriginalPlayerContext;
	public:
		/* Instance */
		static Game*& Instance;
	public:
		#define DEFINE_MEMBER_CLASS Engine::Addresses::Game::Offsets
		union
		{
			DEFINE_MEMBER_B(INT, LocalPlayerId); /* Our localplayer's ID */
			DEFINE_MEMBER_B(Engine::World*, World);
			DEFINE_MEMBER_B(Engine::ResourceManager*, ResourceManager);
			//DEFINE_MEMBER_B(Engine::Camera*, Camera);
			//DEFINE_MEMBER_B(Engine::Map*, Map);

		};
		#undef DEFINE_MEMBER_CLASS

		VOID SpawnUnits(Engine::Player* owner, INT type, Engine::Unit* spawn_near, INT number)
		{
			return static_cast<VOID(__fastcall*)(Engine::Game*, INT, INT, INT, INT)>((void*)Engine::Addresses::Game::Functions::SpawnUnits)(this, owner->Id, type, spawn_near->Id, number);
		}

		VOID SetFogAndBlackmap(BOOL fog, BOOL black)
		{
			static_cast<INT(__fastcall*)(BOOL, BOOL)>((PVOID)Engine::Addresses::Game::Functions::SetFogAndBlackMap)(fog, black);
		}

		Engine::Player* GetLocalPlayer()
		{
			return static_cast<Engine::Player * (__fastcall*)(Engine::Game*)>((PVOID)Engine::Addresses::Game::Functions::GetLocalPlayer)(this);
		}
	};
	Engine::Game*& Engine::Game::Instance = *(Engine::Game**)(Engine::Addresses::Game::Instance);
	DWORD Engine::Game::OriginalPlayerContext = -1;
}