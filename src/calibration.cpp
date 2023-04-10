// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//


#include "calibration.hpp"
#include "Adafruit_SH110X.h"
#include "button.hpp"
#include "distance.hpp"
#include "ui.hpp"

extern Adafruit_SH1107 display;
extern Distance distance;
extern UI ui;

void calibration(bool active) {
    calibration_state state = START;

    while (state != DONE) {
        ui.buttons.read();
        distance.get_distances(false);
        if (active) ui.start_page();

        switch (state) {
            case START: {
                if (active) {
                    if (ui.buttons.o()) {
                        state = CLOSE_LEFT;
                        break;
                    }
                    if (ui.buttons.up() || ui.buttons.down()) {
                        ui.set_current_mode(MENU);
                        return;
                    }
                }
                display.setCursor(0, 8);
                display.setTextSize(2);
                display.write(active ? "Press O\n" : "\n");
                ui.display_values();
                display.display();
                if (!active) return; // A nice goto
                break;
            }
            case CLOSE_LEFT:
                if (ui.buttons.down()) {
                    state = FAR_LEFT;
                    distance.set_left_close();
                }
                display.setCursor(0, 8);
                display.setTextSize(2);
                display.write("Get Close\nPress \\/\n");
                ui.display_value(0);
                display.display();
                break;
            case FAR_LEFT:
                if (ui.buttons.up()) {
                    state = CLOSE_RIGHT;
                    distance.set_left_far();
                }
                display.setCursor(0, 8);
                display.setTextSize(2);
                display.write("Get Far\nPress /\\\n");
                ui.display_value(0);
                display.display();
                break;
            case CLOSE_RIGHT:
                if (ui.buttons.down()) {
                    state = FAR_RIGHT;
                    distance.set_right_close();
                }
                display.setCursor(0, 8);
                display.setTextSize(2);
                display.write("Get Close\nPress \\/\n");
                ui.display_value(1);
                display.display();
                break;
            case FAR_RIGHT:
                if (ui.buttons.up()) {
                    state = DONE;
                    distance.set_right_far();
                }
                display.setCursor(0, 8);
                display.setTextSize(2);
                display.write("Get Far\nPress /\\\n");
                ui.display_value(1);
                display.display();
                break;
            case DONE:
                break;
        }
    }
    ui.set_current_mode(CONTROLLER);
    ui.buttons.wait_all_off();
}
