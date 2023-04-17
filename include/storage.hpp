// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

//
// Created by bjo on 4/16/23.
//

#ifndef THEREMIN_RP2040_STORAGE_HPP
#define THEREMIN_RP2040_STORAGE_HPP

#include <string>
#include <sstream>
#include "LittleFS.h"
#include "distance.hpp"
#include "config.hpp"



class Storage {

public:
    explicit Storage(Distance &distance): distance(distance) {};

    void init() {
        cfg.setAutoFormat(false);
        LittleFS.setConfig(cfg);
        ok = LittleFS.begin();
        if (!ok) {
            reset();
        }
    }

    void reset() {
        LittleFS.format();
        write_config();
        ok=true;
    }

    void write_config() {
        std::stringstream buffer;
        buffer << "version=" << VERSION << "\n";
        buffer << convert_calibration(0, distance.left_calibration);
        buffer << convert_calibration(1, distance.right_calibration);
        auto file = LittleFS.open("config.txt", "w");
        file.write(buffer.str().c_str());
        file.close();
    }

    std::string convert_calibration(uint8_t id, sensor_settings s) {
        std::stringstream buffer;
        //uint16_t close = 20;
        //    uint16_t far = 500;
        //    uint16_t min = 0;
        //    uint16_t max = 127;
        //    uint16_t smooth = 0;
        //    midi_mode mode = CC;
        //    uint8_t channel = 1;
        //    uint8_t controller = 1;
        //    bool inverted = false;
        buffer << "[sensor_" << id << "]" << std::endl;
        buffer << "close=" << s.close << std::endl;
        buffer << "far=" << s.far << std::endl;
        buffer << "min=" << s.min << std::endl;
        buffer << "smooth=" << s.smooth << std::endl;
        buffer << "mode=" << s.mode << std::endl;
        buffer << "channel=" << s.channel << std::endl;
        buffer << "controller=" << s.controller << std::endl;
        buffer << "inverted=" << s.inverted << std::endl;
        return buffer.str();
    }

    std::basic_string<char> get_dirs() {
        std::string foo;
        if (LittleFS.exists("config.txt")) {
            auto file = LittleFS.open("config.txt", "r");
            foo.append(file.readString().c_str());
            file.close();
        }
        return foo;
    }

    bool ok = false;
private:
    LittleFSConfig cfg;
    Distance distance;
};


#endif //THEREMIN_RP2040_STORAGE_HPP
