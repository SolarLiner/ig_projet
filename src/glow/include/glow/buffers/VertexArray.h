//
// Created by solarliner on 12/11/22.
//

#ifndef GLOW_VERTEXARRAY_H
#define GLOW_VERTEXARRAY_H

#include "../Resource.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace glow::buffers {
    class VertexArray : public glow::Resource {
    public:
        VertexArray() { glGenVertexArrays(1, &id); }
        ~VertexArray() { glDeleteVertexArrays(1, &id); }

        void bind() const { glBindVertexArray(id); }

        void unbind() const { glBindVertexArray(0); }

        template<typename V, BufferDraw Draw>
        void bind_buffer(const VertexBuffer<V, Draw> &vbo) const {
            bind();
            vbo.bind();
            V::declare_vertex_attributes();
            unbind();
            vbo.unbind();
        }

        template<typename I, BufferDraw Draw>
        void bind_buffer(const IndexBuffer<I, Draw> &ibo) const {
            bind();
            ibo.bind();
            unbind();
            ibo.unbind();
        }

        explicit operator GLuint() const override { return id; }

    private:
        GLuint id{};
    };

}// namespace glow::buffers

#endif//GLOW_VERTEXARRAY_H
