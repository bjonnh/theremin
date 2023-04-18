// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "../include/distance.hpp"

/**
 * Non blocking get distance
 * @param channel
 * @return
 */
bool Distance::get_distance(uint8_t channel) {
    uint8_t NewDataReady = 0;
    uint8_t status;

    VL53L4CD *device;

    if (channel == 1) {
        device = &cd1;
    } else {
        device = &cd;
    }

    status = device->VL53L4CD_CheckForDataReady(&NewDataReady);
    if (!NewDataReady) return false;

    VL53L4CD_Result_t results;
    if ((!status) && (NewDataReady != 0)) {
        device->VL53L4CD_ClearInterrupt();

        device->VL53L4CD_GetResult(&results);
        if ((results.range_status == 0) & (results.distance_mm > 0)) {
            if (results.distance_mm != old_distances[0]) {
                distances[channel] = results.distance_mm;
                update[channel] = true;
                old_distances[channel] = distances[channel];
            }
        }
    }

    return update[channel];
}

void Distance::init() {
    cd.begin();
    cd.VL53L4CD_Off();
    cd.InitSensor(0x10);
    cd1.begin();
    cd1.VL53L4CD_Off();
    cd1.InitSensor(0x12);


    cd.VL53L4CD_SetRangeTiming(5, 0);
    cd.VL53L4CD_StartRanging();

    cd1.VL53L4CD_SetRangeTiming(5, 0);
    cd1.VL53L4CD_StartRanging();
}

uint16_t Distance::left() {
    return distances[0];
}

uint16_t Distance::right() {
    return distances[1];
}

uint16_t Distance::c_left() {
    return map_distance(distances[0], left_calibration);
}

uint16_t Distance::c_right() {
    return map_distance(distances[1], right_calibration);
}

uint16_t Distance::map_distance(uint16_t d, sensor_settings &calib) {
    if (calib.inverted)
        return constrain(map(d, calib.far, calib.close, calib.min, calib.max), calib.min, calib.max);
    else
        return constrain(map(d, calib.close, calib.far, calib.min, calib.max), calib.min, calib.max);
}

midi_event_t Distance::m_any(midi_event_t &evt, uint16_t distance, sensor_settings &cal) {
    evt.channel = cal.channel;
    evt.controller = cal.controller;
    evt.mode = cal.mode;
    evt.value = distance;
    return evt;
}

void Distance::set_left_close() {
    left_calibration.close = left();
}

void Distance::set_left_far() {
    left_calibration.far = left();
}


void Distance::set_right_close() {
    right_calibration.close = right();
}

void Distance::set_right_far() {
    right_calibration.far = right();
}

void Distance::m_dist(midi_event_t &evt, int i) {
    if (i==1) {
        m_any(evt, c_right(), right_calibration);
    } else {
        m_any(evt, c_left(), left_calibration);
    }
}
