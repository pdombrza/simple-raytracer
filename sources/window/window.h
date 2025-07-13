#pragma once

#include <string>
#include <iostream>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> 

#include "windinput.h"


class Window {
private:
	int registerClass();
	WNDCLASS wc = { sizeof(WNDCLASS) };
	HWND wind = nullptr;
	bool running;
	Input input;
public:
	Window(std::string windowTitle);
	~Window();
	static LRESULT CALLBACK windProcRedirect(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK windProc(HWND windowHandle, UINT msg, WPARAM wp, LPARAM lp);
	void processEventButton(Button& button, bool pressed);
	void processKeyboardAfter();
	void processInputLoop();
	void resetInput();
	void kill();
};