// Copyright (c) 2023. Jonathan Bisson
// SPDX-License-Identifier: MIT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef THEREMIN_RP2040_SETTINGS_HPP
#define THEREMIN_RP2040_SETTINGS_HPP

#include "ui/widget.hpp"

namespace UI::Widgets {
    template<typename D>
    class SettingsWidget : public UI::Widget<D> {
        enum controller_state {
            NORMAL
        };
        controller_state state = NORMAL;

    public:
        explicit SettingsWidget(Widget<D> &parent) : Widget<D>(parent) {}

        void draw() override;

        bool clickAction() override;

        bool leftAction() override;

        bool rightAction() override;

        void action() override;

        void set_state(controller_state state);
    };
}
#endif //THEREMIN_RP2040_SETTINGS_HPP
