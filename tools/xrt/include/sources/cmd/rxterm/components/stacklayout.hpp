#pragma once

#include <algorithm>
#include <sources/cmd/rxterm/image.hpp>
#include <sources/cmd/rxterm/components/component.hpp>
#include <sources/cmd/rxterm/utils.hpp>
#include <sources/cmd/rxterm/style.hpp>
#include <sources/cmd/rxterm/reflow.hpp>

namespace rxterm {

    template<class T = Component> struct StackLayout {
        std::vector<T> children;
        Pixel bg;

        StackLayout(std::vector<T> const &children, Pixel bg = Pixel{}) : children(children), bg{bg} {}

        template<class...Xs> StackLayout(Xs const &...xs)
                : children{xs...}, bg{Pixel{}} {}

        template<class...Xs> StackLayout(Pixel const &bg, Xs const &...xs)
                : children{xs...}, bg{bg} {}

        Image render(int maxWidth) const {
            auto const images = map(children, [maxWidth](auto const &c) {
                return c.render(maxWidth);
            });

            auto const width = std::max_element(images.begin(), images.end(), [](auto const &a, auto const &b) {
                return a.width < b.width;
            })->width;

            auto const height = std::accumulate(images.begin(), images.end(), 0, [](auto const &a, auto const &b) {
                return a + b.height;
            });

            auto canvas = Image::create(width, height, bg);

            auto y = 0;
            for (auto const &image: images) {
                canvas = drawOnBackground(canvas, 0, y, image);
                y += image.height;
            }

            return canvas;
        }
    };
}
