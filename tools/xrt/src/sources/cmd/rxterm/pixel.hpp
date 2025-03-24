#pragma once

#include <string>
#include <sources/cmd/rxterm/style.hpp>

namespace rxterm {

    struct Pixel {
        char c = '\0';
        Style style;

        std::string toString() const {
            return style.toString() + c;
        }
    };
}
