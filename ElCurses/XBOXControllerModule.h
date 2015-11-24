#pragma once

#include <lua.hpp>
#include <unordered_map>
#include <vector>
#include <string>
#include "XBOXController.h"

#define VIBRATEMAX 65535

namespace XBOXControllerModule {
	namespace {
		std::unordered_map<int, XBOXController*> controllerRefs;
		std::vector<std::unordered_map<std::string, bool>> keyMaps(8);
		std::unordered_map<std::string, bool> keyMap{
			{ "A", false }, { "B", false }, { "X", false }, { "Y", false },
			{ "BACK", false }, { "START", false }, { "LB", false }, { "RB", false },
			{ "UP", false }, { "DOWN", false }, { "LEFT", false }, { "RIGHT", false },
			{ "LS", false }, { "RS", false } };
	
		std::vector<std::vector<std::string>> newlyPressed(8);

		void checkNewlyPressed(bool button, int player, const std::string& str) {
			if (button ^ keyMaps[player][str] && button)
				newlyPressed[player].push_back(str);
		}
	}

	std::vector<std::string>& getNewlyPressed(XBOXController* x) {
		return newlyPressed[x->getControllerNumber()];
	}

	void clearNewlyPressed(XBOXController* x) {
		newlyPressed[x->getControllerNumber()].clear();
	}

	static void updateState(XBOXController* x) {
		int player = x->getControllerNumber();

		controllerRefs[player] = x;
		
		// Setup for all players
		if (keyMaps.size() < player) {
			for (int i = 0; i < player - 1; i++)
				keyMaps.push_back(keyMap);
		}

		if (x->IsConnected()) {
			bool button = false;

			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A),				player, "A");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B),				player, "B");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X),				player, "X");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y),				player, "Y");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK),			player, "BACK");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START),			player, "START");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER),	player, "LB");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER), player, "RB");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP),		player, "UP");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN),		player, "DOWN");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT),		player, "LEFT");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT),		player, "RIGHT");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB),	player, "RS");
			checkNewlyPressed((x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB),		player, "LS");


			keyMaps[player]["A"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A;
			keyMaps[player]["B"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B;
			keyMaps[player]["X"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X;
			keyMaps[player]["Y"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y;
			keyMaps[player]["BACK"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
			keyMaps[player]["START"]	= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START;
			keyMaps[player]["LB"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
			keyMaps[player]["RB"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
			keyMaps[player]["UP"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
			keyMaps[player]["DOWN"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
			keyMaps[player]["LEFT"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
			keyMaps[player]["RIGHT"]	= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
			keyMaps[player]["RS"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
			keyMaps[player]["LS"]		= x->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
		}

	}

	static int _isDown(lua_State* L) {
		int player = luaL_checkinteger(L, 1);
		std::string key = luaL_checkstring(L, 2);
		lua_pop(L, 2);

		lua_pushboolean(L, keyMaps[player][key]);
		return 1;
	}

	static int _vibrate(lua_State* L) {
		int player = luaL_checkinteger(L, 1);
		int l = luaL_checkinteger(L, 2);
		int r = luaL_checkinteger(L, 3);
		lua_pop(L, 3);

		XBOXController* ref = controllerRefs[player];
		if (ref)
			ref->Vibrate(l, r);

		return 0;
	}

	static const luaL_Reg functions[] = {
		{ "isDown", _isDown },
		{ "rumble", _vibrate },
		{ 0, 0 }
	};


	static void luaInitModule(lua_State* L) {
		lua_getglobal(L, "el");
		lua_pushstring(L, "gamepad");
		luaL_newlib(L, functions);
		lua_settable(L, -3);

		//luaL_register(L, "el.window", functions);
	}
}
