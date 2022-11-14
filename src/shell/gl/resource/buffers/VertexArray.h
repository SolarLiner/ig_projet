//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_VERTEXARRAY_H
#define IG_PROJET_VERTEXARRAY_H

#include "../Resource.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace shell::gl::resource {
    class VertexArray : public Resource {
    public:
        VertexArray() {
            glGenVertexArrays(1, &id);
        }
        ~VertexArray() {
            glDeleteVertexArrays(1, &id);
        }

        void bind() const {
            glBindVertexArray(id);
        }

        void unbind() const {
            glBindVertexArray(0);
        }

        template<BufferDraw Draw>
        void bind_buffer(const VertexBuffer<Draw> &vbo) const {
            bind();
            vbo.bind();
            Vertex::setup();
            unbind();
            vbo.unbind();
        }

        template<BufferDraw Draw>
        void bind_buffer(const IndexBuffer<Draw> &ibo) const {
            bind();
            ibo.bind();
            unbind();
            ibo.unbind();
        }

        explicit operator GLuint() const override { return id; }

    private:
        GLuint id{};
    };

}// namespace shell::gl::resource

#endif//IG_PROJET_VERTEXARRAY_H
