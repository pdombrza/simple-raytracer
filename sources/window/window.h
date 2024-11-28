#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> 

#include "windinput.h"


class Window {
public:
	Window() {};
	~Window() {};
	static bool running;
	static Input input;
	static LRESULT CALLBACK windProc(HWND windowHandle, UINT msg, WPARAM wp, LPARAM lp);
	static void processEventButton(Button& button, bool pressed);
	static void processKeyboardAfter();
	static void resetInput();
	static void kill();
};