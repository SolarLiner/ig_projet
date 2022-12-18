//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_CAMERA_H
#define IG_PROJET_CAMERA_H

#include "Transform.h"
#include <glow/buffers/UniformBuffer.h>
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <optional>

using glm::mat4;
using glm::quat;
using glm::vec3;

namespace shell::gl {
    using namespace glow::buffers;
    class Camera {
    public:
        Camera();
        void resize(float w, float h);

        [[nodiscard]] float get_fov() const;
        void set_fov(float fov, bool deffered = false);
        [[nodiscard]] float get_aspect_ratio() const;
        void resize(float w, float h, bool deffered = false);
        void resize(glm::vec2 size, bool deffered = false);
        [[nodiscard]] float get_znear() const;
        void set_znear(float znear, bool deffered = false);
        [[nodiscard]] float get_zfar() const;
        void set_zfar(float zfar, bool deffered = false);

        void bind_to(size_t binding);

        void update_projection();

        void update_view();

        void update();

        [[nodiscard]] glm::mat4 get_view() const;
        void set_view(glm::mat4 view, bool deffered = false);

    private:
        glm::mat4 _view{};

        struct matrices {
            matrices(glm::mat4 view, glm::mat4 projection);

            void update_view(glm::mat4 view);

            void update_projection(glm::mat4 projection);

            float view[16];
            float projection[16];
        };
        float fov = M_PI_2f, aspect = 1, znear = 0.01f, zfar = 1e3;
        UniformBuffer<matrices, Dynamic> buffer;
    };

}// namespace shell::gl

#endif//IG_PROJET_CAMERA_H
