//
// Created by solarliner on 12/11/22.
//

#ifndef GLOW_RESOURCE_H
#define GLOW_RESOURCE_H

#include <glad.h>
#include <string>

namespace glow {
    class Resource {
    public:
        Resource(const Resource &) = delete;
        Resource &operator=(const Resource &) = delete;

        virtual explicit operator GLuint() const = 0;

    protected:
        Resource() = default;
    };
}// namespace glow

#endif//GLOW_RESOURCE_H
