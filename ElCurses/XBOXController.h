#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")

class XBOXController {
public:
	XBOXController(int playerNumber) {
		mControllerNumber = playerNumber - 1;
	}

	XINPUT_STATE GetState() {
		ZeroMemory(&mControllerState, sizeof(XINPUT_STATE));
		XInputGetState(mControllerNumber, &mControllerState);
		return mControllerState;
	}

	bool IsConnected() {
		ZeroMemory(&mControllerState, sizeof(XINPUT_STATE));
		DWORD Result = XInputGetState(mControllerNumber, &mControllerState);
		if (Result == ERROR_SUCCESS)
			return true;
		else
			return false;
	}

	void Vibrate(int l = 0, int r = 0) {
		XINPUT_VIBRATION Vibration;
		ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));
		
		Vibration.wLeftMotorSpeed = l;
		Vibration.wRightMotorSpeed = r;

		XInputSetState(mControllerNumber, &Vibration);
	}

	int getControllerNumber() {
		return mControllerNumber;
	}

private:
	XINPUT_STATE mControllerState;
	int mControllerNumber;
};