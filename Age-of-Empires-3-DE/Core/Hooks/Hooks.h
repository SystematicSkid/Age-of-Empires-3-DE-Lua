#pragma once
#include "DirectX.h"
#include "Game.h"

namespace Core
{
	namespace Hooks
	{
		BOOL Initialize(VOID)
		{
			Core::Hooks::DirectX::Initialize();
			Core::Hooks::Game::Initialize();
			return TRUE;
		}
	}
}