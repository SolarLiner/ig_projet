//
// Created by solarliner on 19/11/22.
//

#include "Transform.h"

namespace shell::gl {

    Transform Transform::translation(glm::vec3 amount) {
        return { glm::translate(glm::mat4(), amount)};
    }
    Transform Transform::rotation(glm::quat rot) {
        return { glm::mat4(rot) };
    }
    Transform Transform::rotation(glm::vec3 axis, float angle) {
        return { glm::rotate(angle,axis)};
    }
    Transform Transform::look_at(glm::vec3 pos, glm::vec3 target, glm::vec3 up) {
        return { glm::lookAt(pos, target, up)};
    }
    Transform Transform::scale(glm::vec3 scale) {
        return { glm::scale(scale)};
    }
    Transform Transform::scale(float scale) {
        return Transform::scale(glm::vec3(scale));
    }
    Transform Transform::operator+(const Transform &other) const {
        return { matrix + other.matrix };
    }
    Transform Transform::operator-(const Transform &other) const {
        return { matrix - other.matrix };
    }
    Transform Transform::operator*(const Transform &other) const {
        return { matrix * other.matrix };
    }
}