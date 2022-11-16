//
// Created by solarliner on 15/11/22.
//

#ifndef IG_PROJET_PANORBITCAMERA_H
#define IG_PROJET_PANORBITCAMERA_H

#include "../gl/Camera.h"
#include "glm/glm.hpp"

namespace shell::components {
    using glm::quat;
    using glm::vec2;
    using glm::vec3;
    using glm::vec4;

    static vec3 transform_point(mat4 transform, vec3 point) {
        vec4 w = transform * vec4(point, 1);
        return vec3(w) / w.w;
    }

    class PanOrbitCamera {
    public:
        explicit PanOrbitCamera(vec3 eye = vec3(0, 1, 2), vec3 target = vec3(0))
            : eye(eye), target(target), sensitivity(10), zoom_sensitivity(0.1f) {}
        vec3 eye, target;
        float sensitivity, zoom_sensitivity;

        void update(gl::Camera &camera) { camera.set_view(view()); }

        void zoom(float amount) {
            vec3 dir = glm::normalize(target - eye);
            eye += dir * amount * zoom_sensitivity;
        }

        void pan(vec2 hv) {
            mat4 v = glm::inverse(view());
            vec3 vx = transform_point(v, vec3(0, -1, 0));
            vec3 vy = transform_point(v, vec3(-1, 0, 0));
            hv *= sensitivity;
            quat r = glm::angleAxis(hv.x, vx) * glm::angleAxis(hv.y, vy);
            auto et_dist = glm::distance(eye, target);
            eye = r * eye;
            auto dir = glm::normalize(eye - target);
            eye = et_dist * dir;
        }

        void translate(vec3 world_space) {
            eye += world_space * sensitivity;
            target += world_space * sensitivity;
        }

        void translate(vec2 screen_space) {
            mat4 v = glm::inverse(view());
            translate(transform_point(v, vec3(screen_space, 0)));
        }

    private:
        [[nodiscard]] glm::mat4 view() const { return glm::lookAt(eye, target, vec3(0, 1, 0)); }
    };
}// namespace shell::components

#endif//IG_PROJET_PANORBITCAMERA_H
