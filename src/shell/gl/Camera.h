//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_CAMERA_H
#define IG_PROJET_CAMERA_H

#include "Transform.h"
#include "resource/buffers/UniformBuffer.h"
#include "resource/shaders/Program.h"
#include <glm/detail/type_quat.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <optional>

using glm::mat4;
using glm::quat;
using glm::vec3;

namespace shell::gl {
    class Camera {
    public:
        Camera() : buffer(1) { update(); }
        void resize(float w, float h) { aspect = w / h; }

        [[nodiscard]] float get_fov() const { return fov; }
        void set_fov(float fov, bool deffered = false) {
            Camera::fov = fov;
            if (!deffered) update_projection();
        }
        [[nodiscard]] float get_aspect_ratio() const { return aspect; }
        void resize(float w, float h, bool deffered = false) {
            Camera::aspect = w / h;
            if (!deffered) update_projection();
        }
        void resize(glm::vec2 size, bool deffered = false) { resize(size.x, size.y, deffered); }
        [[nodiscard]] float get_znear() const { return znear; }
        void set_znear(float znear, bool deffered = false) {
            Camera::znear = znear;
            if (!deffered) update_projection();
        }
        [[nodiscard]] float get_zfar() const { return zfar; }
        void set_zfar(float zfar, bool deffered = false) {
            Camera::zfar = zfar;
            if (!deffered) update_projection();
        }

        void bind_to(size_t binding) { buffer.bind_to(binding); }

        void update_projection() {
            buffer.set_at(0, offsetof(matrices, projection),
                          glm::value_ptr(glm::perspective(fov, aspect, znear, zfar)), 16);
        }

        void update_view() { buffer.set_at(0, offsetof(matrices, view), glm::value_ptr(_view), 16); }

        void update() {
            buffer.set(0, matrices(_view, glm::perspective(fov, aspect, znear, zfar)));
        }

        [[nodiscard]] glm::mat4 get_view() const { return _view; }
        void set_view(glm::mat4 view, bool deffered = false) {
            _view = view;
            if (!deffered) update_view();
        }

    private:
        glm::mat4 _view;

        struct matrices {
            matrices(glm::mat4 view, glm::mat4 projection) {
                auto pview = glm::value_ptr(view);
                auto pproj = glm::value_ptr(projection);
                for(int i = 0; i < 16; ++i) {
                    this->view[i] = pview[i];
                    this->projection[i] = pproj[i];
                }
            }
            float view[16];
            float projection[16];
        };
        float fov = M_PI_2f, aspect = 1, znear = 0.01f, zfar = 1e3;
        resource::UniformBuffer<matrices, resource::Dynamic> buffer;
    };

}// namespace shell::gl

#endif//IG_PROJET_CAMERA_H
