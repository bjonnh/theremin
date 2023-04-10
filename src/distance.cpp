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

bool Distance::get_distances(bool blocking) {
    bool update = false;
    uint8_t NewDataReady = 0;
    uint8_t NewDataReady2 = 0;

    uint8_t status, status2;

    VL53L4CD_Result_t results, results2;

    do {
        status = cd.VL53L4CD_CheckForDataReady(&NewDataReady);
        status2 = cd1.VL53L4CD_CheckForDataReady(&NewDataReady2);
    } while (!NewDataReady && !NewDataReady2 && blocking);


    if ((!status) && (NewDataReady != 0)) {
        cd.VL53L4CD_ClearInterrupt();

        cd.VL53L4CD_GetResult(&results);
        if ((results.range_status == 0) & (results.distance_mm > 0)) {
            distances[0] = results.distance_mm;
            update = true;
        }
    }

    if ((!status2) && (NewDataReady2 != 0)) {
        cd1.VL53L4CD_ClearInterrupt();

        cd1.VL53L4CD_GetResult(&results2);
        if ((results2.range_status == 0) & (results2.distance_mm > 0)) {
            distances[1] = results2.distance_mm;
            update = true;
        }
    }
    return update;
}

void Distance::init() {
    cd.begin();
    cd.VL53L4CD_Off();
    cd.InitSensor(0x10);
    cd1.begin();
    cd1.VL53L4CD_Off();
    cd1.InitSensor(0x12);


    cd.VL53L4CD_SetRangeTiming(10, 0);
    cd.VL53L4CD_StartRanging();

    cd1.VL53L4CD_SetRangeTiming(10, 0);
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

uint16_t Distance::map_distance(uint16_t d, Distance::sensor_settings &calib) {
    if (calib.inverted)
        return constrain(map(d, calib.far, calib.close, calib.min, calib.max), calib.min, calib.max);
    else
        return constrain(map(d, calib.close, calib.far, calib.min, calib.max), calib.min, calib.max);
}

midi_event_t Distance::m_left() {
    return m_any(c_left(), left_calibration);
}

midi_event_t Distance::m_right() {
    return m_any(c_right(), right_calibration);
}

midi_event_t Distance::m_any(uint16_t distance, Distance::sensor_settings &cal) {
    midi_event_t entry;
    entry.channel = cal.channel;
    entry.controller = cal.controller;
    entry.mode = cal.mode;
    entry.value = distance;
    return entry;
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
