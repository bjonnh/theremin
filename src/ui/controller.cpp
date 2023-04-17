// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//


#include "ui/uimanager.hpp"
#include "ui/controller.hpp"
#include "distance.hpp"

extern DISPLAY_t display;
extern Distance distance;
extern UIManager ui_manager;

#define COUNT_LIMIT 600

static uint32_t count = 0;

namespace UI::Widgets {
    template
    class ControllerWidget<DISPLAY_t>;

    template<typename D>
    bool ControllerWidget<D>::leftAction() { // left is up
        if (!this->focus) return false;
        switch (state) {
            case NORMAL:
                break;
            case LOW_LATENCY:
                ui_manager.awake();
                set_state(NORMAL);
                break;
        }
        return true;
    }

    template<typename D>
    bool ControllerWidget<D>::rightAction() { // right is down
        if (!this->focus) return false;
        switch (state) {
            case NORMAL:
                set_state(LOW_LATENCY);
                break;
            case LOW_LATENCY:
                set_state(NORMAL);
                break;
        }
        return true;
    }

    template<typename D>
    bool ControllerWidget<D>::clickAction() {
        if (!this->focus) return false;
        switch (state) {
            case NORMAL:
                this->setFocus(false);
                this->exit();
                break;
            case LOW_LATENCY:
                set_state(NORMAL);
                break;
        }
        return true;
    }

    template<typename D>
    void ControllerWidget<D>::action() {
        switch (state) {
            case NORMAL:
                break;
            case LOW_LATENCY:
                if (count < COUNT_LIMIT) {
                    count++;
                } else if (count == COUNT_LIMIT) {
                    ui_manager.low_power();
                }
                break;
        }
    }

    template<typename D>
    void ControllerWidget<D>::set_state(controller_state new_state) {
        if (new_state == state) {
            return;
        }

        switch (new_state) {
            case NORMAL:
                ui_manager.awake();
                break;
            case LOW_LATENCY:
                count = 0;
                break;
        }
        state = new_state;
    }

    template<typename D>
    void ControllerWidget<D>::draw() {
        switch (state) {
            case LOW_LATENCY: {
                display.setCursor(0, 16);
                display.print("       Low latency");
                display.setCursor(0, 24);
                display.print("Display will turn off");
                display.setCursor(0, 36);
                display.print("Press any key to revive");
                break;
            }
            case NORMAL:
                ui_manager.display_values_giant();
                break;
        }
    }
}
