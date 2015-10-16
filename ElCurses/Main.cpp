
#include <curses.h>
#include <iostream>
#include <sys\stat.h>
#include <string>
#include <lua.hpp>
#include <thread>

#include "ConfigParser.h"

const char* workingDirectory = "";

int luaPrint(lua_State* L) {
	int nArgs = lua_gettop(L);

	for (int i = 1; i <= nArgs; i++) {
		if (lua_isnumber(L, i))
			std::cout << lua_tonumber(L, i) << "\t";
		else if (lua_isstring(L, i))
			std::cout << lua_tostring(L, i) << "\t";
		else if (lua_istable(L, i))
			std::cout << "table: 0x" << lua_topointer(L, i) << "\t";
		else if (lua_isfunction(L, i))
			std::cout << "function: 0x" << lua_topointer(L, i) << "\t";
	}
	std::cout << std::endl;
	lua_pop(L, nArgs);

	return 0;
}

int main(int argc, char* argv[]) {
	if (argc == 2) {
		struct stat sb;
		if (stat(argv[1], &sb) == 0) {
			workingDirectory = argv[1];
		}
	}
	
	
	//initscr();

	//noecho(); // TODO: Change to callable from lua

	std::string fname = "config.lua";
	std::string append = "";
	if (argc == 2 && workingDirectory[strlen(workingDirectory) - 1] != '/')
		append = "/";
	
	ConfigParser config;
	//config.parseConfig(std::string(workingDirectory + append + fname).c_str());


	lua_State* L = luaL_newstate();
	bool loaded = false;

	fname = "main.lua";
	struct stat sb;
	if (stat(fname.c_str(), &sb) == 0) {
		luaL_openlibs(L);
		
		// Setup functions
		lua_createtable(L, 0, 1);
		lua_pushstring(L, "update");
		lua_pushnil(L);
		lua_settable(L, -3);
		lua_setglobal(L, "ec");

		lua_register(L, "print", &luaPrint);

		if (luaL_dofile(L, std::string(workingDirectory + append + fname).c_str()) != 0) {
			printf("Lua error: %s \n", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
		else {
		}

		loaded = true;
	}

	bool run = true;

	std::thread update([&]() {
		while (run) {
			lua_getglobal(L, "ec");
			if (lua_istable(L, lua_gettop(L))) {
				lua_pushnil(L);
				while (lua_next(L, -2) != 0) {
					const char* key = lua_tostring(L, -2);
					if (strcmp(key, "update") == 0) {
						float dt = 1.0f / 60.0f;
						if (lua_isfunction(L, lua_gettop(L))) {
							lua_pushnumber(L, dt);
							lua_call(L, 1, 0);
						}
					}
				}
				lua_pop(L, 1);
			}

			wrefresh(stdscr);
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	});
	
	std::thread input([&]() {
		char c = getch();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	});

	update.join();
	input.join();

	endwin();
	return 0;
}