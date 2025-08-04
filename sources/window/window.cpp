#include "window.h"

Window::Window(std::string windowTitle, int width, int height) : buf(width, height) {
	registerClass();

	RECT rect{};
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	wind = CreateWindowExA(
		0,                           // Optional window styles
		wc.lpszClassName,            // Window class
		windowTitle.c_str(),         // Window text
		WS_OVERLAPPEDWINDOW,         // Window style

		CW_USEDEFAULT, CW_USEDEFAULT,

		rect.right - rect.left,      // Calculated width
		rect.bottom - rect.top,      // Calculated height

		NULL,                        // Parent window    
		NULL,                        // Menu
		GetModuleHandle(NULL),       // Instance handle
		this                         // Additional application data
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

void Window::show() const {
	if (wind) {
		ShowWindow(wind, SW_SHOW);
	}
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
	switch (msg) {
	case WM_CLOSE:
		running = false;
		return res;
	case WM_SETFOCUS:
		input.focused = true;
		return res;
	case WM_KILLFOCUS:
		input.focused = false;
		return res;
	case WM_PAINT: {
		PAINTSTRUCT paint;
		HDC hdc = BeginPaint(wind, &paint);
		//HDC hdc = GetDC(wind);
		RECT clientRect;
		GetClientRect(windowHandle, &clientRect);
		int clientWidth = clientRect.right - clientRect.left;
		int clientHeight = clientRect.bottom - clientRect.top;
		//std::cout << "Client size: " << clientWidth << "x" << clientHeight << "\n";
		//std::cout << "Buffer size: " << buf.getWidth() << "x" << buf.getHeight() << "\n";
		auto data = buf.getData();
		//std::cout << "WM_PAINT: Drawing buffer with size " << buf.getWidth() << "x" << buf.getHeight() << std::endl;
		if (data) {
			StretchDIBits(hdc,
				0, 0, buf.getWidth(), buf.getHeight(),
				0, 0, buf.getWidth(), buf.getHeight(),
				data.get(),
				&buf.getBitmapInfo(),
				DIB_RGB_COLORS,
				SRCCOPY
			);
		}
		else {
			std::cerr << "Invalid data!" << std::endl;
		}
		EndPaint(wind, &paint);
		return res;
	} break;
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

void Window::setWindowData(std::shared_ptr<uint8_t[]> data) {
	buf.setData(data);
	InvalidateRect(wind, NULL, TRUE);
}
	

void Window::processInputLoop() {
	while (running) {
		MSG msg = {};
		while (PeekMessage(&msg, wind, NULL, NULL, PM_REMOVE) > 0) // process messages to window from queue
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); // calls the callback to handle events (Window::windproc)
		}

		if (!input.focused) {
			resetInput();
		}

		if (input.keyboard[Button::Q].pressed || input.keyboard[Button::Escape].pressed) {
			kill();
		}

		processKeyboardAfter();
		SendMessage(wind, WM_PAINT, 0, 0);
	}
}

WindowBuffer::WindowBuffer(int w, int h) {
	width = w;
	height = h;
	std::cout << "width: " << width << "height: " << height << std::endl;
	ZeroMemory(&bmi, sizeof(BITMAPINFO));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
}