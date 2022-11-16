//
// Created by solarliner on 15/11/22.
//

#ifndef IG_PROJET_UNIFORMBUFFER_H
#define IG_PROJET_UNIFORMBUFFER_H

#include "Buffer.h"

namespace shell::gl::resource {
    template <typename T, BufferDraw Draw = Static>
    using UniformBuffer = Buffer<T, Uniform, Draw>;
}

#endif//IG_PROJET_UNIFORMBUFFER_H