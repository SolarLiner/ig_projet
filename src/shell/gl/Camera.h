//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_CAMERA_H
#define IG_PROJET_CAMERA_H

#include "Transform.h"
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <optional>

using glm::mat4;
using glm::quat;
using glm::vec3;

namespace shell::gl {
    class Camera {
    public:
        float fov = M_PI_2f, aspect = 1, znear = 0.01f, zfar = 1e3;
        [[nodiscard]] mat4 projection() const { return glm::perspective(fov, aspect, znear, zfar); }
        mat4 &view() { return _view; }

        void resize(float w, float h) {
            aspect = w/h;
        }

    private:
        mat4 _view = glm::identity<mat4>();
    };

}// namespace shell::gl

#endif//IG_PROJET_CAMERA_H
