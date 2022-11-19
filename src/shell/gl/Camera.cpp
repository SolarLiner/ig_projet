//
// Created by solarliner on 19/11/22.
//

#include "Camera.h"

namespace shell::gl {

    Camera::Camera() : buffer(1) { update(); }
    void Camera::resize(float w, float h) { aspect = w / h; }
    float Camera::get_fov() const { return fov; }
    void Camera::set_fov(float fov, bool deffered) {
        Camera::fov = fov;
        if (!deffered) update_projection();
    }
    float Camera::get_aspect_ratio() const { return aspect; }
    void Camera::resize(float w, float h, bool deffered) {
        Camera::aspect = w / h;
        if (!deffered) update_projection();
    }
    void Camera::resize(glm::vec2 size, bool deffered) { resize(size.x, size.y, deffered); }
    float Camera::get_znear() const { return znear; }
    void Camera::set_znear(float znear, bool deffered) {
        Camera::znear = znear;
        if (!deffered) update_projection();
    }
    float Camera::get_zfar() const { return zfar; }
    void Camera::set_zfar(float zfar, bool deffered) {
        Camera::zfar = zfar;
        if (!deffered) update_projection();
    }
    void Camera::bind_to(size_t binding) { buffer.bind_to(binding); }
    void Camera::update_projection() {
        auto map = buffer.map(Write);
        map[0].update_projection(glm::perspective(fov, aspect, znear, zfar));
    }
    void Camera::update_view() {
        auto map = buffer.map(Write);
        map[0].update_view(_view);
    }
    void Camera::update() {
        buffer.set(0, matrices(_view, glm::perspective(fov, aspect, znear, zfar)));
    }
    glm::mat4 Camera::get_view() const { return _view; }
    void Camera::set_view(glm::mat4 view, bool deffered) {
        _view = view;
        if (!deffered) update_view();
    }
    Camera::matrices::matrices(glm::mat4 view, glm::mat4 projection) : view(), projection() {
        update_view(view);
        update_projection(projection);
    }
    void Camera::matrices::update_view(glm::mat4 view) {
        auto p = glm::value_ptr(view);
        for(int i = 0; i < 16; ++i) {
            this->view[i] = p[i];
        }
    }
    void Camera::matrices::update_projection(glm::mat4 projection) {
        auto p = glm::value_ptr(projection);
        for(int i = 0; i < 16; ++i) {
            this->projection[i] = p[i];
        }
    }
}