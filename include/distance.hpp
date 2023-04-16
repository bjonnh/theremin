// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//


#ifndef THEREMIN_RP2040_DISTANCE_HPP
#define THEREMIN_RP2040_DISTANCE_HPP


#include "vl53l4cd_class.h"
#include "midi.hpp"

class Distance {
public:
    void init();

    bool get_distances(bool blocking);

    uint16_t left();

    uint16_t right();

    uint16_t c_left();

    uint16_t c_right();

    midi_event_t m_left();

    midi_event_t m_right();

    struct sensor_settings {
        uint16_t close = 20;
        uint16_t far = 500;
        uint16_t min = 0;
        uint16_t max = 127;
        uint16_t smooth = 0;
        midi_mode mode = CC;
        uint8_t channel = 1;
        uint8_t controller = 1;
        bool inverted = false;
    };

    static uint16_t map_distance(uint16_t d, sensor_settings &calib);


    Distance(VL53L4CD &&cd, VL53L4CD &&cd1) : cd(cd), cd1(cd1) {}

    sensor_settings left_calibration{20, 500, 0, 16383, 0, PITCH_BEND, 1, 1, false};
    sensor_settings right_calibration{20, 500, 0, 127, 0, CC, 1, 1, false};

    void set_left_close();

    void set_left_far();

    void set_right_close();

    void set_right_far();

    bool anyUpdated() {
        return update[0] || update[1];
    }

    bool get_distance(uint8_t channel);

private:
    VL53L4CD cd1;
    VL53L4CD cd;
    uint16_t distances[2]{0, 0};
    uint16_t old_distances[2]{0, 0};
    bool update[2] = {true};

    midi_event_t m_any(uint16_t, sensor_settings &);
};


#endif //THEREMIN_RP2040_DISTANCE_HPP
