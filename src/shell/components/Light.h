//
// Created by solarliner on 15/11/22.
//

#ifndef IG_PROJET_LIGHT_H
#define IG_PROJET_LIGHT_H

#include "../Color.h"
#include <glm/glm.hpp>

namespace shell::components {
    using glm::vec3;

    struct Light {
        enum Kind {
            Point,
            Directional,
            Ambient,
        };
#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
        Light(Kind kind = Ambient, vec3 pos_dir =vec3(0), Color color=Color::Black): kind(kind), dir_or_pos(pos_dir), color_intensity(color) {}
#pragma clang diagnostic pop

        Kind kind;
        char __pad0[12]{};
        vec3 dir_or_pos;
        float __pad1{};
        Color color_intensity;

        static Light point_light(vec3 pos, Color color) { return {Point, pos, color}; }

        static Light directional_light(vec3 dir, Color color) { return {Directional, glm::normalize(dir), color}; }

        static Light ambient(Color color) { return {Ambient, vec3(0), color}; }
    };
}// namespace shell::components


#endif//IG_PROJET_LIGHT_H
