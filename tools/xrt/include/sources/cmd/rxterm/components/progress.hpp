#pragma once

#include <cmath>
#include <sources/cmd/rxterm/style.hpp>
#include <sources/cmd/rxterm/utils.hpp>
#include <sources/cmd/rxterm/image.hpp>

namespace rxterm {

    template<class T> T clamp(T const l, T const r, T const x) {
        return std::min(r, std::max(l, x));
    }

    struct Progress {
        float const progress;
        Pixel const bg;
        Pixel const fg;

        Progress(float const p, Pixel const &bg = Pixel{' ', {Color::Cyan}},
                Pixel const &fg = Pixel{' ', {Color::Blue}}) : progress{p}, bg{bg}, fg{fg} {}

        Image render(unsigned const width) const {
            auto const p = clamp(0.0f, 1.0f, progress);
            return drawOnBackground(Image::create(width, 1, bg), 0, 0,
                    Image::create(static_cast<unsigned int>(width * p), 1, fg));
        }
    };

}
