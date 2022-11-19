//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_TRANSFORM_H
#define IG_PROJET_TRANSFORM_H

#include <glm/gtc/quaternion.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

using glm::mat4;

namespace shell::gl {
    struct Transform {
        static Transform translation(glm::vec3 amount);

        static Transform rotation(glm::quat rot);
        static Transform rotation(glm::vec3 axis, float angle);

        static Transform look_at(glm::vec3 pos, glm::vec3 target, glm::vec3 up = glm::vec3 {0.f, 1.f, 0.f});

        static Transform scale(glm::vec3 scale);
        static Transform scale(float scale);

        Transform operator+(const Transform &other) const;

        Transform operator-(const Transform &other) const;

        Transform operator*(const Transform &other) const;

        mat4 matrix = glm::identity<mat4>();
    };
}// namespace shell::gl::components

#endif//IG_PROJET_TRANSFORM_H
