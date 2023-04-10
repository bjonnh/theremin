// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "../include/ui.hpp"
#include "modes.hpp"

void UI::start_page() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    switch (current_mode) {
        case CALIBRATION:
            display.write("Calibration");
            break;
        case CONTROLLER:
            display.write("Controller");
            break;
        case MENU:
            if (selected_mode==CALIBRATION)
                display.setTextColor(SH110X_BLACK, SH110X_WHITE);
            else
                display.setTextColor(SH110X_WHITE, SH110X_BLACK);
            display.write(" Calib. ");
            if (selected_mode==CONTROLLER)
                display.setTextColor(SH110X_BLACK, SH110X_WHITE);
            else
                display.setTextColor(SH110X_WHITE, SH110X_BLACK);
            display.write(" Cont. ");
            break;
    }
    display.setTextColor(SH110X_WHITE, SH110X_BLACK);
}

void UI::set_current_mode(mode mode1) {
    current_mode = mode1;
    selected_mode = mode1;
}

void UI::menu() {
    if (selected_mode==MENU) // We just entered the menu
        selected_mode = CALIBRATION;
    start_page();
    if (buttons.up()) {
        switch (selected_mode) {
            case CALIBRATION:
                break;
            case CONTROLLER:
                selected_mode = CALIBRATION;
                break;
            case MENU:
                selected_mode = CONTROLLER;
                break;
        }
        buttons.wait_up_off();
    }
    if (buttons.down()) {
        switch (selected_mode) {
            case CALIBRATION:
                selected_mode = CONTROLLER;
                break;
            case CONTROLLER:
                break;
            case MENU:
                selected_mode = CONTROLLER;
                break;
        }
        buttons.wait_down_off();
    }
    if (buttons.o()) {
        current_mode = selected_mode;
        buttons.wait_o_off();
        start_page();
    }
}
