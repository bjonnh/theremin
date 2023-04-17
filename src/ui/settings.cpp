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
#include "ui/settings.hpp"
#include "storage.hpp"

extern DISPLAY_t display;
extern Distance distance;
extern UIManager ui_manager;
extern Storage storage;

#define COUNT_LIMIT 600

static uint32_t count = 0;

namespace UI::Widgets {
    template
    class SettingsWidget<DISPLAY_t>;

    template<typename D>
    bool SettingsWidget<D>::leftAction() { // left is up
        if (!this->focus) return false;
        switch(state) {}
        this->setFocus(false);
        this->exit();

        return true;
    }

    template<typename D>
    bool SettingsWidget<D>::rightAction() { // right is down
        if (!this->focus) return false;
        switch (state) {
        }
        this->setFocus(false);
        this->exit();

        return true;
    }

    template<typename D>
    bool SettingsWidget<D>::clickAction() {
        if (!this->focus) return false;
        switch (state) {
        }
        this->setFocus(false);
        this->exit();

        return true;
    }

    template<typename D>
    void SettingsWidget<D>::action() {
        switch (state) {
        }
    }

    template<typename D>
    void SettingsWidget<D>::set_state(controller_state new_state) {
        if (new_state == state) {
            return;
        }

        switch (new_state) {
        }
        state = new_state;
    }

    template<typename D>
    void SettingsWidget<D>::draw() {
        switch (state) {
            case NORMAL:
                display.setCursor(0,16);
                display.print(storage.get_dirs().c_str());
                display.setCursor(0,24);
                display.println(storage.ok ? "Started" : "Unstarted");
                break;
        }
    }
}
