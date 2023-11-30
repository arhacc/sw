#pragma once

#include <sources/cmd/rxterm/components/component.hpp>
#include <sources/cmd/rxterm/image.hpp>

#include <cmath>
#include <functional>
#include <utility>

namespace rxterm {

auto percent(float const& p) {
    return [=](auto const& width) -> unsigned {
        return p / 100 * width;
    };
}

auto px(unsigned const& width) {
    return [=](auto) -> unsigned {
        return width;
    };
}

struct MaxWidth {
    std::function<unsigned(unsigned)> maxWidth;
    Component const c;

    MaxWidth(std::function<unsigned(unsigned)> maxWidth, Component c)
        : maxWidth{std::move(maxWidth)}, c{std::move(c)} {}

    MaxWidth(unsigned const& maxWidth, Component c)
        : maxWidth{px(maxWidth)}, c{std::move(c)} {}

    Image render(unsigned const width) const {
        return c.render(std::min(width, maxWidth(width)));
    }
};

} // namespace rxterm
