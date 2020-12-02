#pragma once

#include "lua.hpp"

extern "C" {
# include "lua.h"
# include "luajit.h"
# include "lauxlib.h"
# include "lualib.h"
}

#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "luajit.lib")
#define SOL_LUAJIT
#define SOL_LUAJIT_VERSION 99999
#include <sol.hpp>
#define lmax(a,b) (((a) > (b)) ? (a) : (b))
#define lmin(a,b) (((a) < (b)) ? (a) : (b))

namespace Core
{
	namespace Lua
	{
		extern sol::state lua;
	}
}