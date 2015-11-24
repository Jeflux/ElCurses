#pragma once

#include <lua.hpp>
#include <curses.h>

namespace ColorModuleWrapper {
	
	int _initColorPair(lua_State* L) {
		int pair = luaL_checkinteger(L, 1);
		int color1 = luaL_checkinteger(L, 2);
		int color2 = luaL_checkinteger(L, 3);
		lua_pop(L, 3);

		init_pair(pair, color1, color2);
		return 0;
	}

	int _attron(lua_State* L) {
		Window* w = (Window*)lua_touserdata(L, 1);
		int attr = luaL_checkinteger(L, 2);
		lua_pop(L, 2);
		wattron(w->getPDCWindow(), attr);
		return 0;
	}

	int _attroff(lua_State* L) {
		Window* w = (Window*)lua_touserdata(L, 1);
		int attr = luaL_checkinteger(L, 2);
		lua_pop(L, 2);
		wattroff(w->getPDCWindow(), attr);
		return 0;
	}

	static const luaL_Reg functions[] = {
		{ "initPair", _initColorPair },
		{ "attron", _attron },
		{ "attroff", _attroff },
		{ 0, 0 }
	};

	static void luaInitModule(lua_State* L) {
		lua_getglobal(L, "el");
		lua_pushstring(L, "color");
		luaL_newlib(L, functions);
		
		lua_pushstring(L, "black");
		lua_pushnumber(L, 0);
		lua_settable(L, -3);

		lua_pushstring(L, "blue");
		lua_pushnumber(L, 1);
		lua_settable(L, -3);

		lua_pushstring(L, "green");
		lua_pushnumber(L, 2);
		lua_settable(L, -3);

		lua_pushstring(L, "red");
		lua_pushnumber(L, 4);
		lua_settable(L, -3);

		lua_pushstring(L, "white");
		lua_pushnumber(L, 7);
		lua_settable(L, -3);

		lua_settable(L, -3);
	}
}