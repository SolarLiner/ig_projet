//
// Created by solarliner on 12/11/22.
//

#include "Color.h"

namespace shell {
    const Color Color::Black{};
    const Color Color::White{1.f, 1.f, 1.f};
    const Color Color::Red{1.f};
    const Color Color::Green{0.f, 1.f};
    const Color Color::Blue{0.f, 0.f, 1.f};
    const Color Color::Transparent{0.f, 0.f, 0.f, 1.f};
    Color Color::with_alpha(float alpha) const { return Color(r, g, b, alpha); }
}// namespace shell