#pragma once
#include "../Engine.h"

namespace Engine
{
	enum class Resource : int32_t 
	{
		Gold = 0,
		Wood,
		Food,
		Fame,
		SkillPoints,
		XP,
		Ships,
		Trade
	};
}