#pragma once

#include <string>
#include <iostream>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h> 

#include "windinput.h"


struct WindowBuffer {
private:
	std::shared_ptr<uint8_t[]> data = nullptr;
	int width = 0;
	int height = 0;
	BITMAPINFO bmi = {};
public:
	explicit WindowBuffer() = default;
	WindowBuffer(int width, int height);

	int getWidth() const { return width; };
	void setWidth(int w) { width = w; };

	int getHeight() const { return height; };
	void setHeight(int h) { height = h; };

	void setData(std::shared_ptr<uint8_t[]> d) { data = d; };
	std::shared_ptr<uint8_t[]> getData() const { return data; }

	BITMAPINFO& getBitmapInfo() { return bmi; }
};

class Window {
private:
	int registerClass();
	WNDCLASS wc = { sizeof(WNDCLASS) };
	HWND wind = nullptr;
	bool running;
	Input input;
public:
	WindowBuffer buf;
	Window(std::string windowTitle, int width, int height);
	~Window();
	static LRESULT CALLBACK windProcRedirect(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK windProc(HWND windowHandle, UINT msg, WPARAM wp, LPARAM lp);
	void setWindowData(std::shared_ptr<uint8_t[]> data);
	void processEventButton(Button& button, bool pressed);
	void processKeyboardAfter();
	void processInputLoop();
	void resetInput();
	void show() const;
	void kill();
};
