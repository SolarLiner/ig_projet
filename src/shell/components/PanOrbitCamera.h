//
// Created by solarliner on 15/11/22.
//

#ifndef IG_PROJET_PANORBITCAMERA_H
#define IG_PROJET_PANORBITCAMERA_H

#include "../gl/Camera.h"
#include "glm/glm.hpp"

namespace shell::components {
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;
    using glm::quat;

    class PanOrbitCamera {
    public:
        explicit PanOrbitCamera(vec3 eye = vec3(0, 1, -2), vec3 target = vec3(0)): eye(eye), target(target), sensitivity(5), zoom_sensitivity(0.03f) {}
        vec3 eye, target;
        float sensitivity, zoom_sensitivity;

        void update(gl::Camera &camera) {
            camera.set_view(view());
        }

        void zoom(float amount) {
            vec3 dir = glm::normalize(target - eye);
            eye += dir * amount * zoom_sensitivity;
        }

        void pan(vec2 hv) {
            hv *= sensitivity;
            quat r = glm::angleAxis(hv.x, vec3(0, -1, 0)) * glm::angleAxis(hv.y, vec3(-1, 0, 0));
            eye = r * eye;
        }

        void translate(vec3 world_space) {
            eye += world_space * sensitivity;
            target += world_space * sensitivity;
        }

        void translate(vec2 screen_space) {
            mat4 v = glm::inverse(view());
            vec4 w4 = v * vec4 (screen_space, 0, 1);
            vec3 world_space(vec3(w4) / w4.z);
            translate(world_space);
        }

    private:
        [[nodiscard]] glm::mat4 view() const { return glm::lookAt(eye, target, vec3(0, 1, 0)); }
    };
}// namespace shell::system

#endif//IG_PROJET_PANORBITCAMERA_H
