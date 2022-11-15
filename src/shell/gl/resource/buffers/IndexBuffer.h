//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_INDEXBUFFER_H
#define IG_PROJET_INDEXBUFFER_H

#include "Buffer.h"

namespace shell::gl::resource {
    template<BufferDraw Draw = Static>
    using IndexBuffer = Buffer<GLuint, ElementArray, Draw>;
}// namespace shell::gl::resource

#endif//IG_PROJET_INDEXBUFFER_H
