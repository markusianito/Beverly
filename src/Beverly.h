/*
	beverly.hpp
	
	Button Events for AVR Easily (BEVERLY)
	Lightweight, efficient push button event handling for AVR microcontrollers


    Copyright (C) 2025-2026 Marcos Rubiano
	email:	markusianito@proton.me

    Licensed under MIT. See LICENSE file.

*/



#pragma once
#include<Arduino.h>


#define DEBOUNCE_MS 20
#define LONGPRESS_MS 600
#define FLAG_PULL_STATE 	(uint8_t) 0b00010000 // 0 for pulldown, 1 for pullup
#define FLAG_BUTTON_HELD 	(uint8_t) 0b00001000 // if button got held, clears on release.
#define FLAG_BUTTON_READY 	(uint8_t) 0b00000100 // if button got .start()
#define FLAG_LAST_STABLE 	(uint8_t) 0b00000010 // if button has a stable state
#define FLAG_LAST_RAW 		(uint8_t) 0b00000001 // state button from digitalRead

enum class BAction : uint8_t {
	Idle = 0,
	Pressed = 1,
	DoublePressed = 2,
	Held = 3,
	Released = 4,
	NotAvailable = 5
};

class Button {
	private:
	// timeout: ~65s
	uint16_t lastDebounceTime = 0;
    uint16_t pressTime = 0;
	uint8_t info = 0; // bitmask
	uint8_t pin;
	public:
	BAction watch();
	uint8_t getPin();
	void start(uint8_t pull_state=INPUT);
	Button(uint8_t pin);
};