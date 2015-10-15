
#include <curses.h>
#include <iostream>
#include <sys\stat.h>
#include <string>
#include <lua.hpp>
#include <thread>

#include "ConfigParser.h"

const char* workingDirectory = "";

int main(int argc, char* argv[]) {
	if (argc == 2) {
		struct stat sb;
		if (stat(argv[1], &sb) == 0) {
			workingDirectory = argv[1];
		}
	}
	
	
	initscr();

	std::string fname = "config.lua";
	std::string append = "";
	if (argc == 2 && workingDirectory[strlen(workingDirectory) - 1] != '/')
		append = "/";
	
	ConfigParser config;
	config.parseConfig(std::string(workingDirectory + append + fname).c_str());


	lua_State* L = luaL_newstate();
	bool loaded = false;

	fname = "main.lua";
	struct stat sb;
	if (stat(fname.c_str(), &sb) == 0) {
		luaL_openlibs(L);
		if (luaL_dofile(L, std::string(workingDirectory + append + fname).c_str()) != 0) {
			printf("Lua error: %s \n", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
		loaded = true;
	}

	bool run = true;

	std::thread update([&]() {
		while (run) {
			mvprintw(1, 1, "Lol");

			wrefresh(stdscr);
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	});
	

	update.join();

	endwin();
	return 0;
}