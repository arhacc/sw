#pragma once

#include <string>
#include <vector>
#include <algorithm>

namespace rxterm {

    std::vector<std::string> split(std::string const &str, const std::string &delimiter = "\n");

    template<class T, class F> auto map(T const &data, F const &f) {
        std::vector<decltype(f(data[0]))> result(data.size());
        std::transform(data.begin(), data.end(), result.begin(), f);
        return result;
    }

    std::string repeat(unsigned n, std::string const &s);

    std::string clearBeforeCursor();

    std::string clearAfterCursor();

    std::string clearLine();

    std::string moveUp(unsigned n = 1);

    std::string clearLines(unsigned n = 1);

    template<typename T> T clip(const T &n, const T &lower, const T &upper) {
        return std::max(lower, std::min(n, upper));
    }
}
