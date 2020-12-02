#pragma once
#include <windows.h>
#include <vector>
#include <algorithm>
#include <string.h>

/* DirectX */
#pragma warning( push )
#pragma warning( disable : 26495)
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#pragma warning( pop ) 


/* Macros */
#include "../Utility/Macros.h"

/* Addresses */
#include "./Addresses.h"

/* Engine -> Enums */
#include "./Enums/UnitType.h"
#include "./Enums/Resource.h"

/* Engine -> Renderer */
#include "./Classes/Renderer.h"

/* Engine -> Player */
#include "./Classes/Player/Player.h"

/* Engine -> Unit */
#include "./Classes/Unit/ProtoUnit.h"
#include "./Classes/Unit/Unit.h"

/* Classes */
#include "./Classes/ResourceManager.h"
#include "./Classes/World.h"
#include "./Classes/Game.h"