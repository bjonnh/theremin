// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <api/HardwareSPI.h>
#include "button.hpp"
#include "config.hpp"
#include "hardware/gpio.h"

void Buttons::read() {
    btn_up = gpio_get(BTN_UP);
    btn_o = gpio_get(BTN_O);
    btn_down = gpio_get(BTN_DOWN);
}

bool Buttons::up() const {
    return !btn_up;
}

bool Buttons::down() const {
    return !btn_down;
}

bool Buttons::o() const {
    return !btn_o;
}

void Buttons::wait_up_off() {
    while (btn_up == 0) {
        delay(5);
        read();
    }
}

void Buttons::wait_down_off() {
    while (btn_down == 0) {
        delay(5);
        read();
    }
}

void Buttons::wait_o_off() {
    while (btn_o == 0) {
        delay(5);
        read();
    }
}

void Buttons::init() {
    gpio_init(BTN_UP);
    gpio_set_dir(BTN_UP, GPIO_IN);
    gpio_init(BTN_DOWN);
    gpio_set_dir(BTN_DOWN, GPIO_IN);
    gpio_init(BTN_O);
    gpio_set_dir(BTN_O, GPIO_IN);
    gpio_pull_up(BTN_UP);
    gpio_pull_up(BTN_DOWN);
    gpio_pull_up(BTN_O);
}

void Buttons::wait_all_off() {
    while (!btn_o || !btn_up || !btn_down) { read(); }
}
