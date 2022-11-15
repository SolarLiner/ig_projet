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
        Uniform = GL_UNIFORM_BUFFER,
    };
    enum BufferDraw {
        Static = GL_STATIC_DRAW,
        Stream = GL_STREAM_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
    };
    template<typename T, BufferType Type, BufferDraw Draw = Static>
    class Buffer : Resource {
        static_assert(!std::is_pointer_v<T>);
    public:
        Buffer() : Resource() { glGenBuffers(1, &id); }

        Buffer(size_t length) : Buffer() {
            bind();
            glBufferData(Type, sizeof(T) * length, nullptr, Draw);
            unbind();
        }

        Buffer(std::span<T> data) : Buffer() { upload(data); }

        ~Buffer() { glDeleteBuffers(1, &id); }

        void bind() const { glBindBuffer(Type, id); }

        void unbind() const { glBindBuffer(Type, 0); }

        void upload(std::span<T> data) {
            bind();
            length = data.size();
            glBufferData(Type, data.size_bytes(), as_bytes(data).data(), Draw);
            unbind();
        }

        void set(size_t ix, const T &data) {
            bind();
            glBufferSubData(Type, ix * elsize, elsize, (const void *) &data);
            unbind();
        }

        template<typename V>
        void set_at(size_t ix, size_t offset, const V &data) {
            static_assert(!std::is_pointer_v<V>);
            bind();
            glBufferSubData(Type, ix * elsize + offset, sizeof(std::remove_pointer_t<V>), (const void *) &data);
            unbind();
        }

        template<typename V>
        void set_at(size_t ix, size_t offset, const V *data, size_t len) {
            bind();
            glBufferSubData(Type, ix * elsize + offset, sizeof(std::remove_pointer_t<V>) * len, data);
            unbind();
        }

        explicit operator GLuint() const override { return id; }

        [[nodiscard]] size_t size() const { return length; }
        [[nodiscard]] size_t bytes() const { return length * elsize; }

        void bind_to(size_t binding) {
            static_assert(Type == Uniform);
            glBindBufferBase(Type, binding, id);
        }

    private:
        GLuint id = 0;
        size_t length = 0;

        const size_t elsize = sizeof(T);
    };
}// namespace shell::gl::resource

#endif//IG_PROJET_BUFFER_H
