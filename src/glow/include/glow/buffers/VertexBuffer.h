//
// Created by solarliner on 12/11/22.
//

#ifndef GLOW_VERTEXBUFFER_H
#define GLOW_VERTEXBUFFER_H

#include "Buffer.h"

namespace glow::buffers {
    template<typename T, BufferDraw Draw = Static>
    using VertexBuffer = Buffer<T, Array, Draw>;
}// namespace glow::buffers

#endif//GLOW_VERTEXBUFFER_H
