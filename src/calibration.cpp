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
#include "distance.hpp"
#include "uimanager.hpp"

extern DISPLAY_t display;
extern Distance distance;
extern UIManager ui_manager;

namespace UI::Widgets {
    template
    class CalibrationWidget<DISPLAY_t>;

    template<typename D>
    bool CalibrationWidget<D>::leftAction() { // left is up
        if (!this->focus) return false;
        switch (state) {
            case CLOSE_LEFT:
                break;
            case FAR_LEFT:
                set_state(CLOSE_RIGHT);
                distance.set_left_far();
                break;
            case CLOSE_RIGHT:
                break;
            case FAR_RIGHT:
                set_state(DONE);
                distance.set_right_far();
                break;
            case DONE:
            case START:
                set_state(START);
                break;
        }
        return true;
    }

    template<typename D>
    bool CalibrationWidget<D>::rightAction() { // right is down
        if (!this->focus) return false;
        switch (state) {
            case CLOSE_LEFT:
                set_state(FAR_LEFT);
                distance.set_left_close();
                break;
            case FAR_LEFT:
                break;
            case CLOSE_RIGHT:
                set_state(FAR_RIGHT);
                distance.set_right_close();
                break;
            case FAR_RIGHT:
                break;
            case DONE:
            case START:
                set_state(START);
                break;
        }
        return true;
    }

    template<typename D>
    bool CalibrationWidget<D>::clickAction() {
        if (!this->focus) return false;
        switch (state) {
            case START:
                set_state(CLOSE_LEFT);
                break;
            case CLOSE_LEFT:
            case FAR_LEFT:
            case CLOSE_RIGHT:
            case FAR_RIGHT:
                break;
            case DONE:
                set_state(START);
                break;
        }
        return true;
    }

    template<typename D>
    void CalibrationWidget<D>::set_state(calibration_state new_state) {
        if (new_state==START) {
            state = new_state;
            this->setFocus(false);
            this->exit();
        }
        state = new_state;
    }

    template<typename D>
    void CalibrationWidget<D>::action() {
    }

    template<typename D>
    void CalibrationWidget<D>::draw() {
        switch (state) {
            case START:
                display.setCursor(0, 16);
                display.print(this->focus ? "Press O\n" : "\n");
                ui_manager.display_values();
                break;
            case CLOSE_LEFT:
                display.setCursor(0, 16);
                //display.setTextSize(2);
                display.print("Get Close\nPress \\/\n");
                ui_manager.display_value(0);
                break;
            case FAR_LEFT:
                display.setCursor(0, 16);
                //display.setTextSize(2);
                display.print("Get Far\nPress /\\\n");
                ui_manager.display_value(0);
                break;
            case CLOSE_RIGHT:
                display.setCursor(0, 16);
                //display.setTextSize(2);
                display.print("Get Close\nPress \\/\n");
                ui_manager.display_value(1);
                break;
            case FAR_RIGHT:
                display.setCursor(0, 16);
                //display.setTextSize(2);
                display.print("Get Far\nPress /\\\n");
                ui_manager.display_value(1);
                break;
            case DONE:
                display.setCursor(0, 16);
                //display.setTextSize(2);
                display.print("Calibration done. Press O");
                break;
        }
    }
}
