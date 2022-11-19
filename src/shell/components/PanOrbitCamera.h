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
            : eye(eye), target(target), pan_sensitivity(1.f), translate_sensitivity(0.01f), zoom_sensitivity(5.f) {}
        vec3 eye, target;
        float pan_sensitivity, zoom_sensitivity, translate_sensitivity;

        void update(gl::Camera &camera);

        void zoom(float amount);

        void pan(vec2 hv);

        void translate(vec3 world_space);

        void translate(vec2 screen_space);

    private:
        [[nodiscard]] glm::mat4 view() const;
    };
}// namespace shell::components

#endif//IG_PROJET_PANORBITCAMERA_H
