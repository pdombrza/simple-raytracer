#include "window.h"


LRESULT CALLBACK Window::windProc(HWND windowHandle, UINT msg, WPARAM wp, LPARAM lp) {
	LRESULT res = 0;
	switch (msg)
	{
	case WM_CLOSE:
		Window::running = false;
		return res;
	default:
		res = DefWindowProc(windowHandle, msg, wp, lp);
		return res;
	}
}