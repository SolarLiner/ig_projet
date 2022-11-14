//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_BUFFER_H
#define IG_PROJET_BUFFER_H

#include "../Resource.h"
#include "glad.h"
#include <span>

namespace shell::gl::resource {
    enum BufferType {
        Array = GL_ARRAY_BUFFER,
        ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    };
    enum BufferDraw {
        Static = GL_STATIC_DRAW,
        Stream = GL_STREAM_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
    };
    template<typename T, BufferType Type, BufferDraw Draw = Static>
    class Buffer : Resource {
    public:
        Buffer() : Resource() {
            glGenBuffers(1, &id);
        }

        Buffer(std::span<T> data) : Buffer() {
        upload(data);
    }

        ~Buffer() {
            glDeleteBuffers(1, &id);
        }

        void bind() const {
            glBindBuffer(Type, id);
        }

        void unbind() const {
            glBindBuffer(Type, 0);
        }

        void upload(std::span<T> data) {
            bind();
            length = data.size();
            glBufferData(Type, data.size_bytes(),as_bytes(data).data(), Draw);
            unbind();
        }

        explicit operator GLuint() const override { return id; }

        [[nodiscard]] size_t size() const { return length; }
        [[nodiscard]] size_t bytes() const { return length * sizeof(T); }

    private:
        GLuint id = 0;
        size_t length = 0;
    };
}// namespace shell::gl::resource

#endif//IG_PROJET_BUFFER_H
