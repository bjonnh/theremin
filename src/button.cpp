// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <api/ArduinoAPI.h>
#include "button.hpp"
#include "config.hpp"
#include "hardware/gpio.h"


void Buttons::init() {
    static Buttons *local_ref = this;
    gpio_init(BTN_UP); gpio_set_dir(BTN_UP, GPIO_IN); gpio_pull_up(BTN_UP);
    gpio_init(BTN_DOWN); gpio_set_dir(BTN_DOWN, GPIO_IN); gpio_pull_up(BTN_DOWN);
    gpio_init(BTN_O); gpio_set_dir(BTN_O, GPIO_IN); gpio_pull_up(BTN_O);

    btn_up = gpio_get(BTN_UP);
    btn_o = gpio_get(BTN_O);
    btn_down = gpio_get(BTN_DOWN);
    // Using a lambda here to not keep references for stuff around
    gpio_set_irq_enabled_with_callback(BTN_UP, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true,
                                       +[](uint gpio, uint32_t events) -> void {
                                           bool new_value = (events & GPIO_IRQ_EDGE_RISE);
                                           switch (gpio) {
                                               case BTN_UP:
                                                   local_ref->btn_up = new_value;
                                                   break;
                                               case BTN_DOWN:
                                                   local_ref->btn_down = new_value;
                                                   break;
                                               case BTN_O:
                                                   local_ref->btn_o = new_value;
                                                   break;
                                           }
                                       });
    gpio_set_irq_enabled(BTN_DOWN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_O, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
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
