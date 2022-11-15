//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_COLOR_H
#define IG_PROJET_COLOR_H

#include "SFML/Graphics.hpp"
#include <cmath>

namespace shell {
    static float channel_to_linear(float x) {
        if (x < 0.04045) return x / 12.92;
        return pow((x + 0.055) / 1.055, 2.4);
    }
    static float channel_to_srgb(float x) {
        if (x < 0.0031308) return 12.92 * x;
        return 1.055 * pow(x, 1 / 2.4) - 0.055;
    }
    struct Color {
        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Transparent;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
        Color(sf::Color color) : Color(color.r, color.g, color.b, color.a) {}

        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
            : r(r / 255.f), g(g / 255.f), b(b / 255.f), a(a / 255.f) {}

        Color(uint32_t hex) : Color((uint8_t) hex >> 24, (uint8_t) hex >> 16, (uint8_t) hex >> 8, (uint8_t) hex) {}
#pragma clang diagnostic pop

        explicit Color(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) : r(r), g(g), b(b), a(a) {}

        [[nodiscard]] Color to_linear() const {
            return Color(channel_to_linear(r), channel_to_linear(g), channel_to_linear(b), a);
        }

        [[nodiscard]] Color to_srgb() const {
            return Color(channel_to_srgb(r), channel_to_srgb(g), channel_to_srgb(b), a);
        }

        float r, g, b, a;
        Color with_alpha(float alpha) const;
    };
}// namespace shell

#endif//IG_PROJET_COLOR_H
