#pragma once

#include <lua.hpp>
#include "WindowManager.h"

class Window;

namespace WindowModuleWrapper {
	namespace  {
		WindowManager* windowManager;

		Window* getwindow(lua_State* L) {
			Window* w = nullptr;
			if (lua_istable(L, -1)) {
				lua_pushstring(L, "userdata");
				lua_gettable(L, -2);
				w = (Window*)lua_touserdata(L, -1);
			}
			lua_pop(L, 2);

			return w;
		}
	}

	static int _print(lua_State* L) {
		int nArgs = lua_gettop(L);

		Window* w = NULL;
		int x = 0;
		int y = 0;
		const char* str = "";

		switch (nArgs) {
		case 2:
			str = luaL_checkstring(L, 2);
			lua_pop(L, 1);
			
			w = getwindow(L);

			if (w) wprintw(w->getPDCWindow(), str);
			break;
		case 4:
			x = luaL_checkinteger(L, 2);
			y = luaL_checkinteger(L, 3);
			str = luaL_checkstring(L, 4);
			lua_pop(L, 3);

			w = getwindow(L);
			
			if (w) mvwprintw(w->getPDCWindow(), y, x, str);
			break;
		}
		return 0;
	}

	static int _deleteWindow(lua_State* L) {
		Window* w = getwindow(L);
		if (w) windowManager->deleteWindow(w);
		return 0;
	}

	static int _resize(lua_State* L) {
		int x = luaL_checkinteger(L, 2);
		int y = luaL_checkinteger(L, 3);
		lua_pop(L, 2);

		Window* w = getwindow(L);

		if (w) w->resize(x, y);
		return 0;
	}

	static int _setPosition(lua_State* L) {
		int x = lua_tonumber(L, 2);
		int y = lua_tonumber(L, 3);
		lua_pop(L, 2);
		
		Window* w = getwindow(L);

		if (w) w->moveTo(x, y);
		return 0;
	}

	static int _setBorder(lua_State* L) {
		chtype a = luaL_checkinteger(L, 2);
		chtype b = luaL_checkinteger(L, 3);
		chtype c = luaL_checkinteger(L, 4);
		chtype d = luaL_checkinteger(L, 5);
		chtype e = luaL_checkinteger(L, 6);
		chtype f = luaL_checkinteger(L, 7);
		chtype g = luaL_checkinteger(L, 8);
		chtype h = luaL_checkinteger(L, 9);
		lua_pop(L, 8);

		Window* w = getwindow(L);

		if (w) w->setBorder(a, b, c, d, e, f, g, h);
		return 0;
	}


	static int _removeBorder(lua_State* L) {
		Window* w = getwindow(L);
		if (w) w->removeBorder();
		return 0;
	}

	static int _setTitle(lua_State* L) {
		const char* h = luaL_checkstring(L, 2);
		lua_pop(L, 1);

		Window* w = getwindow(L);

		if (w) w->setTitle(h);
		return 0;
	}


	static int _show(lua_State* L) {
		Window* w = getwindow(L);
		if (w) w->setHidden(false);
		return 0;
	}

	static int _hide(lua_State* L) {
		Window* w = getwindow(L);
		if (w) w->setHidden(true);
		return 0;
	}

	static int _top(lua_State* L) {
		Window* w = getwindow(L);
		if (w) w->topPanel();
		return 0;
	}

	static int _bottom(lua_State* L) {
		Window* w = getwindow(L);
		if (w) w->bottonPanel();
		return 0;
	}

	static int _setColor(lua_State* L) {
		int pair = luaL_checkinteger(L, 2);
		lua_pop(L, 1);

		Window* w = getwindow(L);
		if (w) wcolor_set(w->getPDCWindow(), pair, NULL);
		
		return 0;
	}

	static int _touch(lua_State* L) {
		Window* w = getwindow(L);
		if (w) touchwin(w->getPDCWindow());
		return 0;
	}

	static int _getSize(lua_State* L) {
		Window* w = getwindow(L);
		if (w) {
			int width = w->getPDCWindow()->_maxx;
			int height = w->getPDCWindow()->_maxy;

			lua_pushnumber(L, width);
			lua_pushnumber(L, height);
			return 2;
		}
		return 0;
	}

	static int _clear(lua_State* L) {
		Window* w = getwindow(L);
		if (w) {
			WINDOW* window = w->getPDCWindow();
			for (int x = 0; x < window->_maxx; x++)
				for (int y = 0; y < window->_maxy; y++)
					mvwprintw(window, y, x, " ");
		}
		return 0;
	}

	static const luaL_Reg windowFunctions[] = {
		{ "delete", _deleteWindow },
		{ "setSize", _resize },
		{ "setPosition", _setPosition },
		{ "setBorder", _setBorder },
		{ "removeBorder", _removeBorder },
		{ "setTitle", _setTitle },
		{ "print", _print },
		{ "hide", _hide },
		{ "show", _show },
		{ "top", _top },
		{ "bottom", _bottom },
		{ "setColor", _setColor },
		{ "touch", _touch },
		{ "getSize", _getSize },
		{ "clear", _clear },
		{ 0, 0 }
	}; 
	
	static int _createWindow(lua_State* L) {
		int x = luaL_checkinteger(L, 1);
		int y = luaL_checkinteger(L, 2);
		int w = luaL_checkinteger(L, 3);
		int h = luaL_checkinteger(L, 4);
		lua_pop(L, 4);

		Window* window = windowManager->createWindow(w, h, x, y);

		luaL_newlib(L, windowFunctions);
		lua_pushstring(L, "userdata");
		lua_pushlightuserdata(L, window);
		lua_settable(L, -3);

		lua_pushstring(L, "test");
		lua_pushnumber(L, 18);
		lua_settable(L, -3);

		return 1;
	}

	static const luaL_Reg functions[] = {
		{ "create", _createWindow },
		{ 0, 0 }
	};


	static void luaInitModule(lua_State* L, WindowManager* W) {
		windowManager = W;
		lua_getglobal(L, "el");
		lua_pushstring(L, "window");
		luaL_newlib(L, functions);
		lua_settable(L, -3);
	}
}
