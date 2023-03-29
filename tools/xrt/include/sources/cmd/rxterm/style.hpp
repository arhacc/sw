#pragma once

#include <string>

namespace rxterm {

    using std::string;

    enum class Font {
        None = 0, Default = 1, Bold = 1 << 1, // 1
        Faint = 1 << 2, // 2
        Roman = 1 << 3,// not Italic
        Italic = 1 << 4, // 3
        NotUnderline = 1 << 5, Underline = 1 << 6, //4
        Visible = 1 << 7, Hidden = 1 << 8, //8
        Uncrossed = 1 << 9, Crossed = 1 << 10, // 9
        Inherit = 1 << 11
    };

    enum class FontColor {
        None = 0,
        Black = 1,
        Red = 2,
        Green = 3,
        Yellow = 4,
        Blue = 5,
        Magenta = 6,
        Cyan = 7,
        White = 8,
        Default = 9,
        Transparent = 10,
        Inherit = 11,

        BrightBlack = Black + 60,
        BrightRed = Red + 60,
        BrightGreen = Green + 60,
        BrightYellow = Yellow + 60,
        BrightBlue = Blue + 60,
        BrightMagenta = Magenta + 60,
        BrightCyan = Cyan + 60,
        BrightWhite = White + 60
    };


    enum class Color {
        None = 0,
        Black = 1,
        Red = 2,
        Green = 3,
        Yellow = 4,
        Blue = 5,
        Magenta = 6,
        Cyan = 7,
        White = 8,
        Default = 9,
        Transparent = 10,
        Inherit = 11,

        BrightBlack = Black + 60,
        BrightRed = Red + 60,
        BrightGreen = Green + 60,
        BrightYellow = Yellow + 60,
        BrightBlue = Blue + 60,
        BrightMagenta = Magenta + 60,
        BrightCyan = Cyan + 60,
        BrightWhite = White + 60
    };


    template<class X> constexpr auto composeMod(X x) {
        return x;
    }

    template<class X, class...Xs> constexpr auto composeMod(X x, Xs...xs) {
        auto const r = composeMod(xs...);
        auto const delim = (r.empty() || x.empty()) ? "" : ";";
        return x + delim + r;
    }

    template<class X, class...Xs> constexpr auto computeMod(X x, Xs...xs) {
        auto const r = composeMod(x, xs...);
        return (r.empty()) ? "" : "\033[" + r + "m";
    }

    FontColor isStyle(FontColor x);

    Color isStyle(Color x);

    Font isStyle(Font x);

    FontColor toFontColor(FontColor c);

    Color toColor(Color c);

    Font toFont(Font f);

    FontColor toFontColor(...);

    Color toColor(...);

    Font toFont(...);

    template<class...Xs> constexpr FontColor getFontColor(Xs...xs) {
        return (FontColor) std::max({0, (int) toFontColor(xs)...});
    }

    template<class...Xs> constexpr Color getColor(Xs...xs) {
        return (Color) std::max({0, (int) toColor(xs)...});
    }

    Font getFontStyle(Font f = Font::None);


    template<class X, class...Xs> constexpr Font getFontStyle(X x, Xs...xs) {
        return Font(((int) getFontStyle(toFont(x))) | ((int) getFontStyle(toFont(xs)...)));
    }

    bool has(Font x, Font y);

    struct Style {
        Color bg;
        FontColor fg;
        Font font;

        template<class...Styles> constexpr Style(Styles...styles)
                : bg{getColor(isStyle(styles)...)}, fg{getFontColor(isStyle(styles)...)},
                font{getFontStyle(isStyle(styles)...)} {}

        static Style None();

        static Style Default();

        string defaultMod() const;


        string boldMod() const;

        string underlineMod() const;

        string faintMod() const;

        string italicMod() const;

        string hiddenMod() const;

        string crossedMod() const;

        string bgMod() const;

        string fgMod() const;

        std::string toString() const;
    };

    Style diff(Style const &a, Style const &b = Style::None());
}
