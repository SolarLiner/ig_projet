//
// Created by solarliner on 15/11/22.
//

#ifndef GLOW_UNIFORMBUFFER_H
#define GLOW_UNIFORMBUFFER_H

#include "Buffer.h"

namespace glow::buffers {
    template<typename T, BufferDraw Draw = Static>
    using UniformBuffer = Buffer<T, Uniform, Draw>;
}

#endif//GLOW_UNIFORMBUFFER_H