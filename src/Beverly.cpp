/*
    beverly.cpp

    Implements Button Events for AVR Easily.


    Copyright (C) 2025-2026 Marcos Rubiano
	email:	markusianito@proton.me

    SPDX-License-Identifier: MIT
*/

#include "Beverly.h"
#include "Facebits.h"

Button::Button(uint8_t _pin) : pin(_pin){}

uint8_t Button::getPin()
{
    return pin;
}
void Button::start(uint8_t pull_state)
{
    pinMode(pin, pull_state);
    writeData(info, FLAG_BUTTON_READY, true);
    if (pull_state == INPUT_PULLUP)
    {
        writeData(info, FLAG_PULL_STATE, 1);
        writeData(info, FLAG_LAST_RAW, 1);
        writeData(info, FLAG_LAST_STABLE, 1);
    } else
    {
        writeData(info,FLAG_PULL_STATE, 0);
    }
}

BAction Button::watch()
{
    if (!readData(info,FLAG_BUTTON_READY)) {return BAction::NotAvailable;}
    
    uint16_t now = static_cast<uint16_t>(millis());
    bool raw = digitalRead(pin);
    bool pullState = readData(info, FLAG_PULL_STATE);

    if (raw != readData(info, FLAG_LAST_RAW))
    {
        lastDebounceTime = now;
        writeData(info, FLAG_LAST_RAW, raw);
    }

    if (uint16_t(now - lastDebounceTime) > DEBOUNCE_MS)
    {

        if (raw != readData(info, FLAG_LAST_STABLE))
        {
            writeData(info, FLAG_LAST_STABLE, raw);

            if (pullState?!raw:raw) // if pullup, invert raw.
            {   // PRESS
                pressTime = now;
                return BAction::Pressed;
            }
            else
            {   // RELEASE
                writeData(info, FLAG_BUTTON_HELD, 0);
                return BAction::Released;
            }
        }
    }

    // LONG PRESS
    if (readData(info, FLAG_LAST_STABLE))
    {
        if (uint16_t(now - pressTime) >= LONGPRESS_MS)
        {
            writeData(info, FLAG_BUTTON_HELD, 1);
            return BAction::Held;
        }
    }
    return BAction::Idle;
}