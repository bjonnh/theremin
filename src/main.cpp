// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SH110X.h>
#include <vl53l4cd_class.h>
#include "pico/util/queue.h"
#include "config.hpp"
#include "../lib/usbmidi/usbmidi.hpp"
#include "button.hpp"
#include "calibration.hpp"
#include "midi.hpp"
#include "distance.hpp"
#include "controller.hpp"
#include "ui.hpp"

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire1);
VL53L4CD sensor_vl53l4cd_sat(&Wire1, PIN_DETECTOR_LEFT);
VL53L4CD sensor_vl53l4cd_sat2(&Wire1, PIN_DETECTOR_RIGHT);
Distance distance(sensor_vl53l4cd_sat, sensor_vl53l4cd_sat2);
Buttons buttons;
UI ui(display, distance, buttons);

queue_t results_queue;

void setup() {
    buttons.init();
    ui.init();

    queue_init(&results_queue, sizeof(midi_event_t), 8);

    distance.init();
}

UsbMidi usb_midi;

void setup1() {
    usb_midi.init();
    usb_midi(1).begin(MIDI_CHANNEL_OMNI);
}

void loop1() {
    if (queue_get_level(&results_queue) > 0) {
        midi_event_t result;
        // Let's empty the queue and get the last value only
        while (queue_get_level(&results_queue) > 0) {
            queue_remove_blocking(&results_queue, &result);
            if (usb_midi.active()) {
                switch(result.mode) {
                    case CC:
                        usb_midi(1).sendControlChange(result.controller, result.value, result.channel);
                        break;
                    case PITCH_BEND:
                        usb_midi(1).sendPitchBend(result.value-8192, result.channel);
                        break;
                }
            }
        }
    }
}

void loop() {
    buttons.read();
    bool active = (ui.selected_mode == ui.current_mode) && (ui.selected_mode != MENU);
    if (ui.current_mode == MENU)
        ui.menu();

    switch (ui.selected_mode) {
        case CALIBRATION:
            calibration(active);
            break;
        case CONTROLLER:
            controller(active);
            break;
        case MENU:
            break;
    }
}
