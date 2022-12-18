//
// Created by solarliner on 12/11/22.
//

#ifndef GLOW_BUFFER_H
#define GLOW_BUFFER_H

#include "../Resource.h"
#include <glad.h>
#include <span>
#include <vector>

namespace glow::buffers {
    class BufferException : public std::exception {
    public:
        BufferException(std::string msg) : msg(std::move(msg)) {}

        [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }

    private:
        std::string msg;
    };
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
    class Buffer : glow::Resource {
        static_assert(!std::is_pointer_v<T>);

    public:
        struct Map {
            Map(std::span<T> data, const Buffer<T, Type, Draw> &buffer) : ptr(data), buf(buffer) {}

            ~Map() {
                glUnmapBuffer(Type);
                buf.unbind();
            }

            std::span<T> &operator*() { return ptr; }

            T &operator[](size_t ix) { return ptr[ix]; }

            std::span<T> ptr;

        private:
            const Buffer<T, Type, Draw> &buf;
        };
        Buffer() : Resource() { glGenBuffers(1, &id); }

        Buffer(const Buffer &buffer, size_t start = 0, size_t end = 0) : Buffer() {
            glBindBuffer(GL_COPY_READ_BUFFER, (GLuint) buffer);
            glBindBuffer(GL_COPY_WRITE_BUFFER, id);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, (GLintptr) start, (GLintptr) end,
                                buffer.bytes());
        }

        explicit Buffer(size_t length) : Buffer() {
            bind();
            glBufferData(Type, sizeof(T) * length, nullptr, Draw);
            unbind();
        }

        Buffer(std::initializer_list<T> data) : Buffer() { upload(data); }

        ~Buffer() { glDeleteBuffers(1, &id); }

        void bind() const { glBindBuffer(Type, id); }

        void unbind() const { glBindBuffer(Type, 0); }

        void upload(std::span<T> data) {
            bind();
            length = data.size();
            glBufferData(Type, data.size_bytes(), as_bytes(data).data(), Draw);
            unbind();
        }

        void upload(std::initializer_list<T> list) {
            bind();
            auto bytesize = sizeof(T) * list.size();
            length = list.size();
            glBufferData(Type, bytesize, data(list), Draw);
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
            if (!data) throw BufferException("Couldn't map buffer data");
            return {std::span((T *) data, length), *this};
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
}// namespace glow::buffers

#endif//GLOW_BUFFER_H
