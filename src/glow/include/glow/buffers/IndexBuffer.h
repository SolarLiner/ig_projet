//
// Created by solarliner on 12/11/22.
//

#ifndef GLOW_INDEXBUFFER_H
#define GLOW_INDEXBUFFER_H

#include "Buffer.h"
#include <glad.h>

namespace glow::buffers {
    template<typename I> struct element_size_t {
        static const GLenum value;
    };
    template<>
    struct element_size_t<uint8_t> {
        static const GLenum value = GL_UNSIGNED_BYTE;
    };
    template<>
    struct element_size_t<uint16_t> {
        static const GLenum value = GL_UNSIGNED_SHORT;
    };
    template<>
    struct element_size_t<uint32_t> {
        static const GLenum value = GL_UNSIGNED_INT;
    };
    template<typename I, BufferDraw Draw = Static>
    class IndexBuffer : public Buffer<I, ElementArray, Draw> {
        using Buffer<I, ElementArray, Draw>::Buffer;

    public:
        void draw_elements(GLenum mode, bool bound = false) const {
            if (bound) this->bind();
            glDrawElements(mode, this->size(), GL_ELEMENT_TYPE, nullptr);
            if (bound) this->unbind();
        }

    private:
        static const GLenum GL_ELEMENT_TYPE = element_size_t<I>::value;
    };
}// namespace glow::buffers

#endif//GLOW_INDEXBUFFER_H
