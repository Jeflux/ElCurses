#pragma once

#include <lua.hpp>
#include <curses.h>
#include <sstream>
#include "Window.h"

namespace DebugConsoleWrapper {
	class DebugConsole {
	public:
		DebugConsole(Window* window) : 
			mWindow(window),
			mLine(1),
			mHidden(false) {
			window->setTitle("CONSOLE");
			window->setBorder(0, 0, 0, 0, 0, 0, 0, 0);
		}

		void show(bool value) {
			mHidden = !value;
			mWindow->setHidden(!value);
		}

		void setPosition(int x, int y) {
			mWindow->moveTo(x, y);
		}

		void setSize(int w, int h) {
			mWindow->resize(w, h);
		}

		void print(lua_State* L) {
			if (mHidden)
				return;

			int nArgs = lua_gettop(L);

			std::ostringstream stream;
			for (int i = 1; i <= nArgs; i++) {
				if (lua_isnumber(L, i))
					stream << lua_tonumber(L, i) << "\t";
				else if (lua_isstring(L, i))
					stream << lua_tostring(L, i) << "\t";
				else if (lua_istable(L, i))
					stream << "table: 0x" << lua_topointer(L, i) << "\t";
				else if (lua_isfunction(L, i))
					stream << "function: 0x" << lua_topointer(L, i) << "\t";
				else if (lua_isnil(L, i))
					stream << "nil" << "\t";
				else if (lua_isuserdata(L, i))
					stream << "userdata: 0x" << lua_topointer(L, i) << "\t";
				else if (lua_isboolean(L, i))
					stream << (lua_toboolean(L, i) ? "true" : "false") << "\t";
			}
			stream << std::endl;

			wscrl(mWindow->getPDCWindow(), -1);
			mvwprintw(mWindow->getPDCWindow(), 1, 1, stream.str().c_str());

			lua_pop(L, nArgs);
		}

	private:
		Window* mWindow;
		int mLine;
		bool mHidden;
	};

	DebugConsole* instance;

	static int _show(lua_State* L) {
		instance->show(true);
		return 0;
	}

	static int _hide(lua_State* L) {
		instance->show(false);
		return 0;
	}

	static int _setPosition(lua_State* L) {
		float x = (float) luaL_checknumber(L, 1);
		float y = (float) luaL_checknumber(L, 2);
		instance->setPosition(x, y);
		return 0;
	}

	static int _setSize(lua_State* L) {
		float w = (float)luaL_checknumber(L, 1);
		float h = (float)luaL_checknumber(L, 2);
		instance->setSize(w, h);
		return 0;
	}

	void print(lua_State* L) {
		if (instance)
			instance->print(L);
	}

	static const luaL_Reg functions[] = {
		{ "show", _show },
		{ "hide", _hide },
		{ "setPosition", _setPosition },
		{ "setSize", _setSize },
		{ 0, 0 }
	};

	static void luaInitConsole(lua_State* L, Window* window) {
		instance = new DebugConsole(window);
		lua_getglobal(L, "el");
		lua_pushstring(L, "console");
		luaL_newlib(L, functions);
		lua_settable(L, -3);
	}
}