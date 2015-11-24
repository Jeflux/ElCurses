// Include xinput on Windows
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include "XBOXController.h"
#include "XBOXControllerModule.h"
#endif

#include <curses.h>
#include <sys\stat.h>
#include <string>
#include <lua.hpp>
#include <thread>
#include <unordered_map>
#include <mutex>

#include "ConfigParser.h"
#include "ConsoleModule.h"
#include "WindowManager.h"
#include "WindowModule.h"
#include "ColorModule.h"


const char* workingDirectory = "";
bool run = true;

int luaPrint(lua_State* L) {
	DebugConsoleWrapper::print(L);
	return 0;
}

int luaExit(lua_State* L) {
	run = false;
	return 0;
}

int main(int argc, char* argv[]) {
	const char* tableName = "el";
	std::mutex luaAccess;
	
	if (argc == 2) {
		struct stat sb;
		if (stat(argv[1], &sb) == 0) {
			workingDirectory = argv[1];
		}
	}

	initscr();
	keypad(stdscr, true);
	start_color();

	noecho(); // TODO: Change to callable from lua

	std::unordered_map<char, bool> keys(128);

	std::string fname = "config.lua";
	std::string append = "";
	if (argc == 2 && workingDirectory[strlen(workingDirectory) - 1] != '/')
		append = "/";
	
	ConfigParser config;
	config.parseConfig(std::string(workingDirectory + append + fname).c_str());

	WindowManager windowManager;

	lua_State* L = luaL_newstate();
	bool loaded = false;

	fname = "main.lua";
	struct stat sb;
	if (stat(fname.c_str(), &sb) == 0) {
		luaL_openlibs(L);
		
		// Setup functions
		lua_newtable(L);
		lua_pushnil(L);
		lua_setfield(L, -2, "update");
		lua_pushnil(L);
		lua_setfield(L, -2, "draw");
		lua_pushnil(L);
		lua_setfield(L, -2, "key");
		lua_pushnil(L);
		lua_setfield(L, -2, "gamepadDown");
		lua_pushcfunction(L, luaExit);
		lua_setfield(L, -2, "exit");
		lua_setglobal(L, tableName);
		
		lua_register(L, "print", &luaPrint);

		DebugConsoleWrapper::luaInitConsole(L, windowManager.createWindow(0, 0, 0, 0));
		WindowModuleWrapper::luaInitModule(L, &windowManager);
		ColorModuleWrapper::luaInitModule(L);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		XBOXControllerModule::luaInitModule(L);
#endif
		
		
		if (luaL_dofile(L, std::string(workingDirectory + append + fname).c_str()) != 0) {
			//printf("Lua error: %s \n", lua_tostring(L, -1));
			//lua_pop(L, 1);
		}
		else {
		}

		loaded = true;
	}

	std::thread update([&]() {
		while (run) {			
			luaAccess.lock();
			lua_getglobal(L, tableName);
			lua_getfield(L, -1, "update");
			if (lua_isfunction(L, lua_gettop(L))) {
				float dt = 1.0f / 60.0f;
				lua_pushnumber(L, dt);
				lua_call(L, 1, 0);
			}
			lua_settop(L, 0);

			lua_getglobal(L, tableName);
			lua_getfield(L, -1, "draw");
			if (lua_isfunction(L, lua_gettop(L))) {
				lua_call(L, 0, 0);
			}
			lua_settop(L, 0);

			windowManager.refresh();
			
			luaAccess.unlock();
			
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	});

	std::thread input([&]() {
		while (run) {
			int c = getch(); // Blocking
			unsigned long m = PDC_get_key_modifiers();

			luaAccess.lock();
			lua_getglobal(L, tableName);
			lua_getfield(L, -1, "key");
			if (lua_isfunction(L, lua_gettop(L))) {
				lua_pushnumber(L, c);
				lua_pushstring(L, keyname(c));
				lua_call(L, 2, 0);
			}
			lua_settop(L, 0);
			luaAccess.unlock();

			//std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	});


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	XBOXController pc1(1);

	std::thread gamepadInput([&]() {
		while (run) {
			XBOXControllerModule::updateState(&pc1);
			auto newlyPressed = XBOXControllerModule::getNewlyPressed(&pc1);
			if (newlyPressed.size() > 0) {
				luaAccess.lock();
				for (auto n : newlyPressed) {
					lua_getglobal(L, tableName);
					lua_getfield(L, -1, "gamepadDown");
					if (lua_isfunction(L, lua_gettop(L))) {
						lua_pushnumber(L, 0);
						lua_pushstring(L, n.c_str());
						lua_call(L, 2, 0);
					}
				}
				lua_settop(L, 0);
				luaAccess.unlock();

				XBOXControllerModule::clearNewlyPressed(&pc1);
			}
			

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	});
#endif

	update.join();
	input.join();

	endwin();
	return 0;
}