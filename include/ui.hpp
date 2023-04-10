// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef THEREMIN_RP2040_UI_HPP
#define THEREMIN_RP2040_UI_HPP


#include "Adafruit_SH110X.h"
#include "distance.hpp"
#include "modes.hpp"
#include "button.hpp"

class UI {
public:
    explicit UI(Adafruit_SH1107 &sh1107, Distance &distance, Buttons &buttons) : display(sh1107), distance(distance), buttons(buttons) {};

    void init() {
        display.begin(0x3C, true);
        display.clearDisplay();
        display.display();
        display.setRotation(3);
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0, 0);
        display.write("Booting");
        display.display();
    }

    void display_values() {
        snprintf(report, sizeof(report),
                 "< %4u\n  %4u >", distance.c_left(), distance.c_right()
        );

        display.setTextSize(2);
        display.setCursor(0, 24);
        display.write(report);
    }

    void display_value(size_t index) {
        snprintf(report, sizeof(report), "%s%3u mm%s", index == 0 ? "< " : "  ",
                 index==0 ? distance.left() : distance.right(), index == 1 ? " >" : "");
        display.setTextSize(2);
        display.setCursor(0, 48);
        display.write(report);
    }

    void display_values_giant() {
        snprintf(report, sizeof(report), "<%3u\n\n      %3u>", distance.c_left(), distance.c_right());
        display.setTextSize(2);
        display.setCursor(0, 16);
        display.write(report);
        display.setTextSize(1);
        display.setCursor(90, 16);
        if (distance.left_calibration.mode == PITCH_BEND) {
            display.write("PITCH");
        } else {
            snprintf(report, sizeof(report), "CC %u", distance.left_calibration.controller);
            display.write(report);
        }
        display.setCursor(90, 24);
        snprintf(report, sizeof(report), "Ch %u", distance.left_calibration.channel);
        display.write(report);

        display.setCursor(0, 46);

        if (distance.right_calibration.mode == PITCH_BEND) {
            display.write("PITCH");
        } else {
            snprintf(report, sizeof(report), "CC %u", distance.right_calibration.controller);
            display.write(report);
        }

        display.setCursor(0, 54);
        snprintf(report, sizeof(report), "Ch %u", distance.right_calibration.channel);
        display.write(report);
        display.drawFastHLine(0, 9, 128, SH110X_WHITE);
        display.drawFastHLine(0, 40, 128, SH110X_WHITE);
    }

    mode current_mode = MENU;
    mode selected_mode = CALIBRATION;

    void set_current_mode(mode mode1);

    void start_page();

    void menu();

    Buttons &buttons;
private:
    Adafruit_SH1107 &display;
    char report[64];
    Distance &distance;

};


#endif //THEREMIN_RP2040_UI_HPP
