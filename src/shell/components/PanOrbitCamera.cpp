//
// Created by solarliner on 19/11/22.
//

#include "PanOrbitCamera.h"

namespace shell::components {
    static vec3 transform_point(mat4 transform, vec3 point) {
        auto w = transform * vec4(point, 1);
        return vec3(w) / w.w;
    }

    static vec3 transform_vec(mat4 transform, vec3 vec) {
        vec4 w = transform * vec4(vec, 0);
        return {w};
    }

    void PanOrbitCamera::update(gl::Camera &camera) { camera.set_view(view()); }

    void PanOrbitCamera::zoom(float amount) {
        vec3 dir = glm::normalize(target - eye);
        eye += dir * amount * zoom_sensitivity;
    }

    void PanOrbitCamera::pan(vec2 hv) {
        mat4 v = glm::inverse(view());
        vec3 vx = transform_vec(v, vec3(0, -1, 0));
        vec3 vy = transform_vec(v, vec3(-1, 0, 0));
        hv *= pan_sensitivity;
        quat r = glm::angleAxis(hv.x, vx) * glm::angleAxis(hv.y, vy);
        auto et_dist = glm::distance(eye, target);
        eye = r * eye;
        auto dir = glm::normalize(eye - target);
        eye = et_dist * dir;
    }

    void PanOrbitCamera::translate(vec3 world_space) {
        eye += world_space * translate_sensitivity;
        target += world_space * translate_sensitivity;
    }

    void PanOrbitCamera::translate(vec2 screen_space) {
        mat4 v = glm::inverse(view());
        translate(transform_point(v, vec3(screen_space, 0)));
    }

    glm::mat4 PanOrbitCamera::view() const { return glm::lookAt(eye, target, vec3(0, 1, 0)); }
}