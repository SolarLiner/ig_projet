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
    enum BufferMapping {
        Read = GL_READ_ONLY,
        Write = GL_WRITE_ONLY,
    };
    template<typename T, BufferType Type, BufferDraw Draw = Static>
    class Buffer : Resource {
        static_assert(!std::is_pointer_v<T>);
    public:
        struct Map {
            Map(std::span<T> data, const Buffer<T, Type, Draw> &buffer): ptr(data), buf(buffer) {}

            ~Map() {
                glUnmapBuffer(Type);
                buf.unbind();
            }

            std::span<T> &operator*() {
                return ptr;
            }

            T &operator[](size_t ix) {
                return ptr[ix];
            }

            std::span<T> ptr;
        private:
            const Buffer<T, Type, Draw> &buf;
        };
        Buffer() : Resource() { glGenBuffers(1, &id); }

        Buffer(const Buffer &buffer, size_t start = 0, size_t end = 0): Buffer() {
            glBindBuffer(GL_COPY_READ_BUFFER, (GLuint) buffer);
            glBindBuffer(GL_COPY_WRITE_BUFFER, id);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, (GLintptr)start, (GLintptr) end, buffer.bytes());
        }

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

        Map map(BufferMapping mapping) {
            bind();
            auto data = glMapBuffer(Type, mapping);
            return {std::span((T*)data, length), *this};
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
