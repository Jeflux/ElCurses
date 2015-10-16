#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <lua.hpp>
#include <fstream>

struct ConfigParser {
public:
	void parseConfig(const char* fileName) {
		lua_State* L = luaL_newstate();

		// Check if file found
		std::ifstream infile(fileName);
		if (!infile.good())
			return;

		luaL_openlibs(L);
		if (luaL_dofile(L, fileName) != 0) {
			printf("Lua error: %s \n", lua_tostring(L, -1));
			lua_pop(L, 1);
		}

		int width = stdscr->_maxx;
		int height = stdscr->_maxy;
		bool resize = false;
		resize |= get(L, &width, "width");
		resize |= get(L, &height, "height");
		if (resize)
			resize_term(width, height);

		const char* title = "";
		if (get(L, &title, "title"))
			PDC_set_title(title);

		bool hideCursor;
		if (get(L, &hideCursor, "cursor"))
			curs_set(hideCursor);

		lua_close(L);
	}

private:
	static bool get(lua_State* L, int* result, const char* name) {
		lua_getglobal(L, name);
		bool ret = false;
		if (lua_isnumber(L, -1)) {
			*result = lua_tointeger(L, -1);
			ret = true;
		}
		lua_pop(L, 1);
		return ret;
	}

	static bool get(lua_State* L, const char** result, const char* name) {
		lua_getglobal(L, name);
		bool ret = false;
		if (lua_isstring(L, -1)) {
			*result = lua_tostring(L, -1);
			ret = true;
		}
		lua_pop(L, 1);
		return ret;
	}

	static bool get(lua_State* L, bool* result, const char* name) {
		lua_getglobal(L, name);
		bool ret = false;
		if (lua_isboolean(L, -1)) {
			*result = lua_toboolean(L, -1);
			ret = true;
		}
		lua_pop(L, 1);
		return ret;
	}
};


#endif CONFIGPARSER_H