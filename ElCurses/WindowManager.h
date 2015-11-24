#pragma once

#include <curses.h>
#include <vector>
#include <algorithm>
#include "Window.h"
#include <unordered_map>

class WindowManager {
public:
	WindowManager() : mMainWindow(newwin(0, 0, 0, 0)) {
		int b = 128;
		//mMainWindow.setBorder(b, b, b, b, b, b, b, b);
	}

	Window* createWindow(int w, int h, int x, int y) {
		WINDOW* window = newwin(h, w, y, x);
		Window* n = new Window(window);
		scrollok(window, true);
		mWindows.push_back(n);
		return n;
	}

	void deleteWindow(Window* w) {
		mWindows.erase(std::remove(mWindows.begin(), mWindows.end(), w), mWindows.end());
		w->dispose();
		delete w;
	}

	void refresh() {
		// Use window with panel as background for efficient clearing
		touchwin(mMainWindow.getPDCWindow());
		
		std::unordered_map<int, std::vector<Window*>> sortMap;

		mMainWindow.applyBorder();
		for (auto w : mWindows) {
			if (w->isHidden()) 
				continue;

			int z = w->getZ();
			sortMap[z].push_back(w);
			WINDOW* window = w->getPDCWindow();
			w->applyBorder();
		}

		/*for (int i = 0; i < sortMap.size(); i++) {
			for (int j = 0; j < sortMap[i].size(); j++) {
				Window* w = sortMap[i][j];
				WINDOW* window = w->getPDCWindow();
				w->applyBorder();
				//wrefresh(window);
			}
		}*/


		update_panels();
		doupdate();
	}

private:
	std::vector<Window*> mWindows;
	Window mMainWindow;
};