//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_VERTEXBUFFER_H
#define IG_PROJET_VERTEXBUFFER_H

#include "Buffer.h"
#include "Vertex.h"

namespace shell::gl::resource {
    template<BufferDraw Draw = Static>
    class VertexBuffer : public Buffer<Vertex, Array, Draw> {};
}// namespace shell::gl::resource

#endif//IG_PROJET_VERTEXBUFFER_H
