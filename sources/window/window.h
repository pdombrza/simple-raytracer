#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> 


class Window {
public:
	Window() {};
	~Window() {};
	static bool running;
	static LRESULT CALLBACK windProc(HWND windowHandle, UINT msg, WPARAM wp, LPARAM lp);
};