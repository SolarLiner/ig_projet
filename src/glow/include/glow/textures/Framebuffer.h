//
// Created by solarliner on 20/11/22.
//

#ifndef IG_PROJET_FRAMEBUFFER_H
#define IG_PROJET_FRAMEBUFFER_H

#include "Texture.h"
#include "glow/Resource.h"

#include <glad.h>

namespace glow::textures {
    class Framebuffer : Resource {
    public:
        static const Framebuffer DEFAULT_TARGET;

        Framebuffer();

        ~Framebuffer();

        template<typename T>
        void attach_color(const Texture<T, D2> &texture, size_t slot) const {
            bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, (GLuint) texture, 0);
        }

        template<typename T>
        void attach_depth(const Texture<depth<T>, D2> &texture) const {
            bind();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, (GLuint) texture, 0);
        }

        void bind() const;

        void unbind() const;

    private:
        explicit operator GLuint() const override;

    private:
        explicit Framebuffer(GLuint id);
        GLuint id{};
    };
}// namespace glow::textures

#endif//IG_PROJET_FRAMEBUFFER_H
