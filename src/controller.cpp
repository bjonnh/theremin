// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//


#include "Adafruit_SH110X.h"
#include "button.hpp"
#include "ui.hpp"

extern Adafruit_SH1107 display;
extern Buttons buttons;
extern Distance distance;
extern UI ui;

extern queue_t results_queue;

uint32_t count = 0;

uint32_t COUNT_LIMIT = 1000;

void controller(bool active) {
    static bool low_latency = false;
    static uint16_t old_distances[2]{0, 0};
    if (active) {
        if (buttons.o()) {
            if (low_latency) {
                low_latency = false;
                ui.buttons.wait_o_off();
                return;
            }
            buttons.wait_o_off();
            ui.set_current_mode(MENU);
            return;
        }
        if (buttons.up()) { // UP
            if (low_latency) {
                low_latency = false;
                ui.buttons.wait_up_off();
                return;
            }
            low_latency = true;
            count = 0;
            ui.start_page();
            display.write("\nLow latency\n");
            display.write("Display will turn off\n");
            display.setTextSize(2);
            display.write("Press any \nto revive");
            display.display();
            buttons.wait_up_off();
            return;
        }
        if (buttons.down()) {
            if (low_latency) {
                low_latency = false;
                ui.buttons.wait_down_off();
                return;
            }
            low_latency = false;
            buttons.wait_up_off();
            return;
        }
    }
    if (low_latency && (count++ >= COUNT_LIMIT)) {
        display.clearDisplay();
        display.display();
    }

    bool update = distance.get_distances(true);

    if (update) {
        if (!low_latency) {
            if (active) ui.start_page();
            ui.display_values_giant();
            display.display();
        }
        if (distance.left() != old_distances[0]) {
            auto ent = distance.m_left();
            queue_add_blocking(&results_queue, &ent);
            old_distances[0] = distance.left();
        }

        if (distance.right() != old_distances[1]) {
            auto ent = distance.m_right();
            queue_add_blocking(&results_queue, &ent);
            old_distances[0] = distance.right();
        }
    }
}
