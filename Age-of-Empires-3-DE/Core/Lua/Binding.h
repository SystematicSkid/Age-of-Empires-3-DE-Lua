#pragma once
#include <fstream>
#include <ostream>
#define SOL_CHECK_ARGUMENTS 1

namespace Core
{
	namespace Lua
	{

		class Binding
		{
		private:
			/*
				Get lua folder
			*/
			static std::string GetLuaDirectory()
			{

				char path[MAX_PATH];
				GetCurrentDirectoryA(MAX_PATH, path);
				if (!path)
					return nullptr;

				auto string = std::string(path) + ("\\lua");
				auto attrib = GetFileAttributesA(string.c_str());

				if (attrib == INVALID_FILE_ATTRIBUTES || !(attrib & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (!CreateDirectoryA(string.c_str(), NULL))
						return nullptr;
				}
				return string + "\\";
			}

			static bool LoadFile(std::string file)
			{
				file = GetLuaDirectory() + file;
				printf("\t[ Lua ] Loading: %s\n", file.c_str());
				sol::protected_function_result result = lua.script_file(file, sol::script_pass_on_error);
				//c_assert(!result.valid());
				if (!result.valid())
				{
					sol::error err = result;
					//sol::call_status status = result.status();
					printf("\n%s\n", err.what());

					return false;
				}

				return true;
			}

			static void UserTypes()
			{
				// Engine Types

				lua.new_usertype<Engine::Game>("Game",
					"LocalPlayerId", &Engine::Game::LocalPlayerId,
					"World", &Engine::Game::World,
					"ResourceManager", &Engine::Game::ResourceManager,
					"GetLocalPlayer", &Engine::Game::GetLocalPlayer,
					"SpawnUnits", &Engine::Game::SpawnUnits,
					"SetFogAndBlackmap", &Engine::Game::SetFogAndBlackmap
					);				
				lua.new_usertype<Engine::ResourceManager>("ResourceManager",
					"NumPrototypes", &Engine::ResourceManager::NumPrototypes,
					"GetPrototype", &Engine::ResourceManager::GetPrototype
					);

				lua.new_usertype<Engine::World>("World",
					"NumUnits", &Engine::World::NumObjects,
					"MaxUnits", &Engine::World::MaxObjects,
					"GetUnit", &Engine::World::GetUnit,
					"NumPlayers", &Engine::World::NumPlayers,
					"MaxPlayers", &Engine::World::MaxPlayers,
					"GetPlayer", &Engine::World::GetPlayer
					);

				lua.new_usertype<Engine::ProtoUnit>("Prototype",
					"Id", &Engine::ProtoUnit::Id,
					"Name", &Engine::ProtoUnit::Name
					);

				lua.new_usertype<Engine::Unit>("Unit",
					"Id", &Engine::Unit::Id,
					"GetPosition", &Engine::Unit::GetPosition,
					"GetPrototype", &Engine::Unit::GetPrototype,
					"GetOwner", &Engine::Unit::GetOwner,
					"WorldToScreen", &Engine::Unit::WorldToScreen,
					"GetHitpoints", &Engine::Unit::GetHitpoints
					);

				lua.new_usertype<Engine::Player>("Player",
					"Id", &Engine::Player::Id,
					"Age", &Engine::Player::Age,
					"GetPopulationCount", &Engine::Player::GetPopulationCount,
					"GetPopulationCap", &Engine::Player::GetPopulationCap,
					"GetResourceAmount", &Engine::Player::GetResourceAmount,
					"SetResource", &Engine::Player::SetResource
					);

				/* Math */
				lua.new_usertype<D3DXVECTOR3>("Vector3",
					sol::constructors<D3DXVECTOR3(), D3DXVECTOR3(float, float, float)>(),
					"x", &D3DXVECTOR3::x,
					"y", &D3DXVECTOR3::y,
					"z", &D3DXVECTOR3::z,
					"__tostring", [](D3DXVECTOR3& v)
					{
						return "[" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + "]";
					}
				);

				lua.new_usertype<D3DXVECTOR2>("Vector2",
					sol::constructors<D3DXVECTOR2(), D3DXVECTOR2(float, float)>(),
					"x", &D3DXVECTOR2::x,
					"y", &D3DXVECTOR2::y,
					"__tostring", [](D3DXVECTOR2& v)
					{
						return "[" + std::to_string(v.x) + ", " + std::to_string(v.y) +  "]";
					}
				);
			}

			static void Enums()
			{
				/* Enums */
				lua.new_enum("Resource",
					"Gold", Engine::Resource::Gold,
					"Wood", Engine::Resource::Wood,
					"Food", Engine::Resource::Food,
					"Fame", Engine::Resource::Fame,
					"Skillpoints", Engine::Resource::SkillPoints,
					"XP", Engine::Resource::XP,
					"Ships", Engine::Resource::Ships,
					"Trade", Engine::Resource::Trade
				);
			}
		public:

			template<typename... Args>
			static void HookCall(std::string name, Args&... args)
			{
				try { lua["hook"]["Call"](name, args...); }
				catch (std::exception& e) { printf("%s\n", e.what()); }
			}

			static bool LoadHooks()
			{
				std::string extensions = GetLuaDirectory() + ("\\extensions");
				auto attrib = GetFileAttributesA(extensions.c_str());

				if (attrib == INVALID_FILE_ATTRIBUTES || !(attrib & FILE_ATTRIBUTE_DIRECTORY))
				{
					if (!CreateDirectoryA(extensions.c_str(), NULL))
						return false;
					std::ofstream outfile(GetLuaDirectory() + ("extensions\\hook.lua"));
					outfile << "hook = {};\r\n\r\nlocal Hooks = {};\r\n\r\nfunction hook.GetTable()\r\n\treturn Hooks;\r\nend\r\n\r\nfunction hook.Call(name, a, b, c, d, e, f, g)\r\n\tfor k,v in next, (Hooks[name] or {}), nil do\r\n\t\tlocal ret = {v(a, b, c, d, e, f, g)}\r\n\t\tif ret[1] then\r\n\t\t\treturn table.unpack(ret)\r\n\t\tend\r\n\tend\r\nend\r\n\r\nfunction hook.Add(name, id, func)\r\n\r\n\tHooks[name] = Hooks[name] or {};\r\n\tHooks[name][id] = func;\r\n\t\r\nend\r\n\r\nfunction hook.Remove(name, id)\r\n\r\n\thook.Add(name, id, nil);\r\n\t\r\nend";
					outfile.close();
				}
				printf("\t\t[ Lua ] Loading hook.lua...\n");
				if (!LoadFile(("extensions\\hook.lua")))
					return false;
			}

			static bool Initialize()
			{
				lua = {}; //reset state
				lua.open_libraries(sol::lib::base,
					sol::lib::string,
					sol::lib::coroutine,
					sol::lib::os,
					sol::lib::math,
					sol::lib::table,
					sol::lib::bit32,
					sol::lib::io,
					sol::lib::package,
					sol::lib::jit);

				UserTypes();
				Enums();

				printf("\t[ Lua ] Directory: %s\n", GetLuaDirectory().c_str());
				if (!LoadHooks())
				{
					printf("\t[ Lua ] Failed to load hooks!\n");
					return false;
				}

				lua.set_exception_handler([](lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) -> INT
					{
						printf("An exception has occurred!");
						if (maybe_exception)
						{
							printf("error: ");
							const std::exception& ex = *maybe_exception;
							printf("%s\n", ex.what());
						}
						else
						{
							printf("description: ");
							printf("%s\n", description.data());
						}

						return sol::stack::push(L, description);
					});

				lua.set_function("include", [](std::string path) { lua.script_file(path); });
				lua.set_function("print", [](const char* msg) {printf(msg); });

				printf("\t[ Lua ] Loading scripts...\n");
				std::ifstream file(GetLuaDirectory() + ("loadorder.txt"));
				std::string str;
				bool result = true;
				while (std::getline(file, str))
				{
					result = LoadFile(str);
					printf("\t[ Lua ] %s: Loaded = %s\n", str.c_str(), result ? "true" : "false");
				}

				HookCall("PostExecute", Engine::Game::Instance);

				return true;
			}
		};

		sol::state lua = sol::state();
	}
}
