// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "../include/uimanager.hpp"
#include "modes.hpp"

UIManager *current_manager;

void set_current_position(uint8_t position) {
    current_manager->page_calibration.setVisible(position == 0);
}

void enter_page(uint8_t position) {
    current_manager->main_menu.setFocus(false);
    current_manager->page_calibration.setFocus(position == 0);
}

void re_enter_menu() {
    current_manager->page_calibration.setFocus(false);
    current_manager->main_menu.setFocus(true);
}


void UIManager::init() {
    current_manager = this;
    display.setBusClock(800000);
    display.begin();
    display.sendF("ca", 0x81, 0x4f);
    display.sendF("ca", 0xa8, 0x7f);
    display.clearBuffer();
    display.setPowerSave(0);
    display.setContrast(180);  // This is extremely important
    display.setFontMode(0);
    display.setDrawColor(1);
    display.setFont(u8g2_font_5x8_mr);
    display.setCursor(0, 8);
    display.print("Booting");
    display.sendBuffer();

    root.setVisible(true);
    main_menu.setVisible(true);
    re_enter_menu();
    main_menu.set_highlighted_item_to(0);
    main_menu.set_selected_item_to(0);

    main_menu.addItem((char *) "Ctrl");
    main_menu.addItem((char *) "Calib");
    main_menu.addItem((char *) "Set");
    main_menu.onHighlightedCall(&set_current_position);
    main_menu.onSelectedCall(&enter_page);

  //  calibration.onExitCall(&re_enter_menu);
}

void UIManager::start_page() {
    return;
    clear();

    display.setCursor(0, 8);
    //display.setTextSize(1);
    switch (current_mode) {
        case CALIBRATION:
            display.print("Calibration");
            break;
        case CONTROLLER:
            display.print("Controller");
            break;
        case MENU:
            if (selected_mode==CALIBRATION)
                display.setDrawColor(0);
            else

                display.setDrawColor(1);

            display.print(" Calib. ");
            if (selected_mode==CONTROLLER)
                display.setDrawColor(0);
            else
                display.setDrawColor(1);
            display.print(" Cont. ");
            break;
    }
    display.setDrawColor(0);
}

void UIManager::set_current_mode(mode mode1) {
    current_mode = mode1;
    selected_mode = mode1;
}

void UIManager::menu() {
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

void UIManager::display_values() {
    snprintf(report, sizeof(report),
             "< %4u\n  %4u >", distance.c_left(), distance.c_right()
    );

    //display.setTextSize(2);
    display.setCursor(0, 24);
    display.print(report);
}

void UIManager::display_value(size_t index) {
    snprintf(report, sizeof(report), "%s%3u mm%s", index == 0 ? "< " : "  ",
             index==0 ? distance.left() : distance.right(), index == 1 ? " >" : "");
    //display.setTextSize(2);
    display.setCursor(0, 48);
    display.print(report);
}

void UIManager::display_values_giant() {
    snprintf(report, sizeof(report), "<%3u\n\n      %3u>", distance.c_left(), distance.c_right());
    //display.setTextSize(2);
    display.setCursor(0, 16);
    display.print(report);
    //display.setTextSize(1);
    display.setCursor(90, 16);
    if (distance.left_calibration.mode == PITCH_BEND) {
        display.print("PITCH");
    } else {
        snprintf(report, sizeof(report), "CC %u", distance.left_calibration.controller);
        display.print(report);
    }
    display.setCursor(90, 24);
    snprintf(report, sizeof(report), "Ch %u", distance.left_calibration.channel);
    display.print(report);

    display.setCursor(0, 46);

    if (distance.right_calibration.mode == PITCH_BEND) {
        display.print("PITCH");
    } else {
        snprintf(report, sizeof(report), "CC %u", distance.right_calibration.controller);
        display.print(report);
    }

    display.setCursor(0, 54);
    snprintf(report, sizeof(report), "Ch %u", distance.right_calibration.channel);
    display.print(report);
    display.drawHLine(0, 9, 128);
    display.drawHLine(0, 40, 128);
}

void UIManager::commit() {
    display.sendBuffer();
}

void UIManager::clear() {
    display.clearBuffer();
}
int i=0;
void UIManager::update() {
    update_inputs();
    clear();
    root.draw();
    commit();
}

void UIManager::update_inputs() {
    buttons.read();
    if (buttons.o()) click();
    else if (buttons.down()) down();
    else if (buttons.up()) up();
    buttons.wait_all_off();
}

void UIManager::click() {
    root.click();
}

void UIManager::down() {
    root.move_right();
}

void UIManager::up() {
    root.move_left();
}

UIManager::UIManager(DISPLAY_t &display, Distance &distance, Buttons &buttons) :
display(display), distance(distance), buttons(buttons),root(display), main_menu(root), page_calibration(root), calibration(page_calibration)
{

}
