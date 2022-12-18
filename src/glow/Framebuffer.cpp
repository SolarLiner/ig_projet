//
// Created by solarliner on 20/11/22.
//

#include "glow/textures/Framebuffer.h"

namespace glow::textures {
    Framebuffer::Framebuffer() { glGenFramebuffers(1, &id); }
    Framebuffer::~Framebuffer() {
        if (id > 0) glDeleteFramebuffers(1, &id);
    }
    void Framebuffer::bind() const { glBindFramebuffer(GL_FRAMEBUFFER, id); }
    void Framebuffer::unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    Framebuffer::operator GLuint() const { return id; }
    Framebuffer::Framebuffer(GLuint id) : id(0) {}
    const Framebuffer Framebuffer::DEFAULT_TARGET = Framebuffer(0);
}// namespace glow::textures
