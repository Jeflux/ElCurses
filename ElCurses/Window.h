#pragma once

#include <curses.h>
#include <panel.h>

class Window {
public:
	Window(WINDOW* window) : 
		mPDCWindow(window),
		mHasTitle(false),
		mHasBorder(false) {
		mPDCPanel = new_panel(window);
		show_panel(mPDCPanel);
	}

	void setBorder(chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br) {
		mHasBorder = true;
		mLs = ls;
		mRs = rs;
		mTs = ts;
		mBs = bs;
		mTl = tl;
		mTr = tr;
		mBl = bl;
		mBr = br;
	}

	void removeBorder() {
		mHasBorder = false;
		int b = (int) ' ';
		wborder(mPDCWindow, b, b, b, b, b, b, b, b);
	}

	void applyBorder() {
		if (mHasBorder)
			wborder(mPDCWindow, mLs, mRs, mTs, mBs, mTl, mTr, mBl, mBr);
		
		if (mHasTitle)
			mvwprintw(mPDCWindow, 0, 1, mTitle);
	}

	WINDOW* getPDCWindow() {
		return mPDCWindow;
	}

	void moveTo(int x, int y) {	
		mX = x;
		mY = y;
		
		move_panel(mPDCPanel, y, x);
		//mvwin(mPDCWindow, y, x);
	}

	void resize(int w, int h) {
		WINDOW* newWin = resize_window(mPDCWindow, h, w);
		if (newWin != nullptr) {
			mW = w;
			mH = h;
			mPDCWindow = newWin;
			move_panel(mPDCPanel, mY, mX);
			//mvwin(mPDCWindow, mY, mX);
		}
	}

	void setTitle(const char* title) {
		mHasTitle = true;
		mTitle = title;
	}

	void removeTitle() {
		mHasTitle = false;
	}

	bool isHidden() {
		return mHidden;
	}

	void setHidden(bool value) {
		mHidden = value;

		if (value)
			hide_panel(mPDCPanel);
		else
			show_panel(mPDCPanel);
	}

	void topPanel() {
		top_panel(mPDCPanel);
	}

	void bottonPanel() {
		top_panel(mPDCPanel);
	}

	void setZ(int z) {
		mZOrder = z;
	}

	int getZ() {
		return mZOrder;
	}

	void dispose() {
		del_panel(mPDCPanel);
		delwin(mPDCWindow);
	}

private:
	WINDOW* mPDCWindow;
	PANEL* mPDCPanel;
	
	bool mHasTitle;
	const char* mTitle;

	// Border
	bool mHasBorder;
	chtype mLs;
	chtype mRs;
	chtype mTs;
	chtype mBs;
	chtype mTl;
	chtype mTr;
	chtype mBl;
	chtype mBr;

	int mX;
	int mY;
	int mW;
	int mH;

	bool mHidden;

	int mZOrder;
};