#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct Button {
	Button() {};
	~Button() {};
	bool pressed = false;
	bool held = false;
	bool released = false;

	enum {
		Q = 0,
		Escape,
		BUTTON_COUNT,
	};

	static constexpr int buttonValues[BUTTON_COUNT] = {
		'Q', VK_ESCAPE,
	};
};


struct Input {
	Input() {};
	~Input() {};
	Button keyboard[Button::BUTTON_COUNT];
	bool focused = false;
};


