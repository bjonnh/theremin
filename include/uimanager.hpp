// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef THEREMIN_RP2040_UIMANAGER_HPP
#define THEREMIN_RP2040_UIMANAGER_HPP

#include "U8g2lib.h"
#include "distance.hpp"
#include "modes.hpp"
#include "button.hpp"
#include "ui/root.hpp"
#include "ui/horizontalmenu.hpp"
#include "calibration.hpp"
#include "ui/page.hpp"

typedef U8G2 DISPLAY_t;

class UIManager {
public:
    explicit UIManager(DISPLAY_t &display, Distance &distance, Buttons &buttons);;

    void init();

    void display_values();

    void display_value(size_t index);

    void display_values_giant();

    mode current_mode = MENU;
    mode selected_mode = CALIBRATION;

    void set_current_mode(mode mode1);

    void start_page();

    void menu();

    Buttons &buttons;

    void commit();

    void clear();

    void update();


    UI::Widgets::Root<DISPLAY_t> root;
    UI::Widgets::Horizontal_menu<DISPLAY_t> main_menu;
    UI::Widgets::Page<DISPLAY_t> page_calibration;
    UI::Widgets::CalibrationWidget<DISPLAY_t> calibration;
private:
    DISPLAY_t &display;
    char report[64]{0};
    Distance &distance;



    void update_inputs();

    void click();

    void down();

    void up();
};


#endif //THEREMIN_RP2040_UIMANAGER_HPP
