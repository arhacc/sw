//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "sources/cmd/rxterm/utils.hpp"

std::vector<std::string> split(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> tokens;

    auto start = 0U;
    auto end = str.find(delimiter);
    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }

    if (start != str.size())
        tokens.push_back(str.substr(start, str.size() - start));

    return tokens;
}

std::string rxterm::repeat(unsigned int n, const std::string &s) {
    std::string result;
    for (unsigned i = 0; i < n; ++i) {
        result += s;
    }
    return result;
}

std::string rxterm::clearBeforeCursor() {
    return "\033[0K";
}

std::string rxterm::clearAfterCursor() {
    return "\033[1K";
}

std::string rxterm::clearLine() {
    return "\033[2K\r";
}

std::string rxterm::moveUp(unsigned int n) {
    return "\033[" + std::to_string(n) + "A\r";
}

std::string rxterm::clearLines(unsigned int n) {
    return "\033[0m" + clearBeforeCursor() + ((n) ? repeat(n, clearLine() + moveUp()) : std::string(""));
}
