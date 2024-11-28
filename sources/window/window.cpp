#include "window.h"

void Window::processEventButton(Button& button, bool pressed) {
	if (pressed) {
		if (!button.held) {
			button.pressed = true;
			button.held = true;
			button.released = false;
		}
	}
	else {
		button.pressed = false;
		button.held = false;
		button.released = true;
	}
}

void Window::processKeyboardAfter() {
	for (int i = 0; i < Button::BUTTON_COUNT; i++) {
		input.keyboard[i].pressed = 0;
		input.keyboard[i].released = 0;
	}
}

void Window::resetInput() {
	ZeroMemory(input.keyboard, sizeof(input.keyboard)); // win32 api macro for memset
}

void Window::kill() {
	running = false;
}

LRESULT CALLBACK Window::windProc(HWND windowHandle, UINT msg, WPARAM wp, LPARAM lp) {
	LRESULT res = 0;
	bool pressed = false;
	switch (msg)
	{
	case WM_CLOSE:
		Window::running = false;
		return res;
	case WM_SETFOCUS:
		input.focused = true;
		return res;
	case WM_KILLFOCUS:
		input.focused = false;
		return res;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		pressed = true;
	case WM_SYSKEYUP:
	case WM_KEYUP: {
		for (int i = 0; i < Button::BUTTON_COUNT; i++) {
			if (wp == Button::buttonValues[i]) {
				processEventButton(input.keyboard[i], pressed);
			}
		}
	}
	default:
		res = DefWindowProc(windowHandle, msg, wp, lp);
		return res;
	}
}