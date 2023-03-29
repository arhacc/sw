//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "sources/cmd/rxterm/style.hpp"

rxterm::FontColor rxterm::isStyle(rxterm::FontColor x) { return x; }

rxterm::Color rxterm::isStyle(rxterm::Color x) { return x; }

rxterm::Font rxterm::isStyle(rxterm::Font x) { return x; }

rxterm::FontColor rxterm::toFontColor(rxterm::FontColor c) { return c; }

rxterm::Color rxterm::toColor(rxterm::Color c) { return c; }

rxterm::Font rxterm::toFont(rxterm::Font f) { return f; }

rxterm::FontColor rxterm::toFontColor(...) { return FontColor::None; }

rxterm::Color rxterm::toColor(...) { return Color::None; }

rxterm::Font rxterm::toFont(...) { return Font::None; }

rxterm::Font rxterm::getFontStyle(rxterm::Font f) {
    return f;
}

bool rxterm::has(rxterm::Font x, rxterm::Font y) {
    return (((int) x) & ((int) y));
}

rxterm::Style rxterm::diff(const rxterm::Style &a, const rxterm::Style &b) {
    bool keepBG = (a.bg == b.bg);
    bool keepFG = (a.fg == b.fg);
    bool keepFont = (a.font == b.font);

    int l = (int) a.font;
    int r = (int) b.font;
    //  bool reset = (l & ~r) ? true : false;
    bool reset = (l & ~r);

    return Style{(keepBG && !reset) ? Color::Inherit : b.bg, (keepFG && !reset) ? FontColor::Inherit : b.fg,
            (keepFont && !reset) ? Font::Inherit : (Font) ((r & ((l & r) ^ r)) | (static_cast<int>(reset)))};
}

rxterm::Style rxterm::Style::None() {
    return {};
}

rxterm::Style rxterm::Style::Default() {
    return {Font::Default};
}

std::string rxterm::Style::defaultMod() const {
    return has(font, Font::Default) ? "0" : "";
}

std::string rxterm::Style::boldMod() const {
    return has(font, Font::Bold) ? "1" : "";
}

std::string rxterm::Style::underlineMod() const {
    return has(font, Font::Underline) ? "4" : "";
}

std::string rxterm::Style::faintMod() const {
    return has(font, Font::Faint) ? "2" : "";
}

std::string rxterm::Style::italicMod() const {
    return has(font, Font::Italic) ? "3" : "";
}

std::string rxterm::Style::hiddenMod() const {
    return has(font, Font::Hidden) ? "8" : "";
}

std::string rxterm::Style::crossedMod() const {
    return has(font, Font::Crossed) ? "9" : "";
}

std::string rxterm::Style::bgMod() const {
    return ((int) bg != 11) ? ((!(int) bg) ? "0" : std::to_string(40 + (int) bg - 1)) : "";
}

std::string rxterm::Style::fgMod() const {
    if (!(int) fg) return "0";
    return ((int) fg != 11) ? ((!(int) bg) ? "0" : std::to_string(30 + (int) fg - 1)) : "";
}

std::string rxterm::Style::toString() const {
    return computeMod(defaultMod(), boldMod(), underlineMod(), faintMod(), italicMod(), hiddenMod(), crossedMod(),
            bgMod(), fgMod());
}
