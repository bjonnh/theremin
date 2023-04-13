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

extern UIManager ui_manager;
extern queue_t results_queue;

UIManager::UIManager(DISPLAY_t &display, Distance &distance, Buttons &buttons) :
        display(display), distance(distance),
        buttons(buttons),
        root(display),
        main_menu(root),
        page_calibration(root), calibration(page_calibration),
        page_controller(root), controller(page_controller)
{}

void set_current_position(uint8_t position) {
    ui_manager.page_calibration.setVisible(position == 0);
    ui_manager.page_controller.setVisible(position == 1);
}

void enter_page(uint8_t position) {
    ui_manager.main_menu.setFocus(false);
    ui_manager.page_calibration.setFocus(position == 0);
    ui_manager.page_controller.setFocus(position == 1);
}

void re_enter_menu() {
    ui_manager.page_calibration.setFocus(false);
    ui_manager.page_controller.setFocus(false);
    ui_manager.main_menu.setFocus(true);
}

void UIManager::init() {
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

    main_menu.addItem((char *) "Ctrl");
    main_menu.addItem((char *) "Calib");
    main_menu.addItem((char *) "Set");
    main_menu.onHighlightedCall(&set_current_position);
    main_menu.onSelectedCall(&enter_page);
    re_enter_menu();
    main_menu.set_highlighted_item_to(0);
    main_menu.set_selected_item_to(0);


    calibration.onExitCall(&re_enter_menu);
    controller.onExitCall(&re_enter_menu);
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

void UIManager::update() {
    update_inputs();
    root.action();
    bool distances_updated = distance.get_distances(false);

    if (distances_updated) {
        const bool *updates = distance.isUpdated();
        if (updates[0]) {
            auto ent = distance.m_left();
            queue_add_blocking(&results_queue, &ent);
        }

        if (updates[1]) {
            auto ent = distance.m_right();
            queue_add_blocking(&results_queue, &ent);
        }

        distance.clear_update();
    }

    if (to_update || distances_updated) {
        if (!power_save) {
            clear();
            root.draw();
            commit();
        }
        to_update = false;
    }
}

void UIManager::update_inputs() {
    buttons.read();
    if (buttons.o()) click();
    else if (buttons.down()) down();
    else if (buttons.up()) up();
    buttons.wait_all_off();
}

void UIManager::click() {
    trigger_update();
    root.click();
}

void UIManager::down() {
    trigger_update();
    root.move_right();
}

void UIManager::up() {
    trigger_update();
    root.move_left();
}

void UIManager::low_power() {
    trigger_update();
    power_save=true;
    display.setPowerSave(1);
}

void UIManager::awake() {
    trigger_update();
    power_save=false;
    display.setPowerSave(0);
}

void UIManager::trigger_update() {
    to_update = true;
}
