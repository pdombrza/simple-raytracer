#include "window.h"

Window::Window(std::string windowTitle) {
	registerClass();
	wind = CreateWindowExA(
		0, // optional window style
		wc.lpszClassName, // class name
		TEXT(windowTitle.c_str()), // window title
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, // style (default)

		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // posx, posy, sizex, sizey
		NULL, NULL, GetModuleHandle(NULL), this // parent, menu, handle, ptr to this
	);
}

Window::~Window() {
	DestroyWindow(wind);
	PostQuitMessage(0);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

int Window::registerClass() {
	running = true;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = Window::windProcRedirect;
	wc.hInstance = GetModuleHandle(NULL); // handle to the calling process (so my program)
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.lpszClassName = "RT Window Class";

	if (!RegisterClass(&wc)) {
		std::cerr << "Failed to register window class";
		return 1;
	}
}

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

LRESULT CALLBACK Window::windProcRedirect(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NCCREATE) {
		CREATESTRUCT* create = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* window = reinterpret_cast<Window*>(create->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
		window->wind = hwnd;
	}

	Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (window) {
		return window->windProc(hwnd, msg, wParam, lParam); // Call non-static member function
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::windProc(HWND windowHandle, UINT msg, WPARAM wp, LPARAM lp) {
	LRESULT res = 0;
	bool pressed = false;
	switch (msg)
	{
	case WM_CLOSE:
		running = false;
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
		[[fallthrough]];
	case WM_SYSKEYUP:
	case WM_KEYUP: {
		for (int i = 0; i < Button::BUTTON_COUNT; i++) {
			if (wp == Button::buttonValues[i]) {
				processEventButton(input.keyboard[i], pressed);
			}
		}
	}
	[[fallthrough]];
	default:
		res = DefWindowProc(windowHandle, msg, wp, lp);
		return res;
	}
}

void Window::processInputLoop() {
	while (Window::running) {
		MSG msg = {};
		while (PeekMessage(&msg, wind, NULL, NULL, PM_REMOVE) > 0) // process messages to window from queue
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); // calls the callback to handle events (Window::windproc)
		}

		if (!Window::input.focused) {
			Window::resetInput();
		}

		if (Window::input.keyboard[Button::Q].pressed || Window::input.keyboard[Button::Escape].pressed) {
			Window::kill();
		}

		Window::processKeyboardAfter();
	}
}