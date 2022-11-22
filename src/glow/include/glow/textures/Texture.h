//
// Created by solarliner on 20/11/22.
//

#ifndef IG_PROJET_TEXTURE_H
#define IG_PROJET_TEXTURE_H

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glow/Resource.h"

#include <span>

namespace glow::textures {
    template<typename T>
    struct depth {};

    template<typename Repr>
    struct gl_repr {
//        static_assert(false, "Repr not supported");
        static const GLenum internal_format;
        static const GLenum type;
        static const GLenum format;
        static const GLenum read_format;
    };

    template<>
    struct gl_repr<glm::vec3> {
        static const GLenum internal_format = GL_RGB32F;
        static const GLenum type = GL_FLOAT;
        static const GLenum format = GL_RGB;
        static const GLenum read_format = GL_RGB;
    };

    template<>
    struct gl_repr<glm::vec4> {
        static const GLenum internal_format = GL_RGBA32F;
        static const GLenum type = GL_FLOAT;
        static const GLenum format = GL_RGBA;
        static const GLenum read_format = GL_RGBA;
    };

    template<>
    struct gl_repr<uint32_t> {
        static const GLenum internal_format = GL_R32UI;
        static const GLenum type = GL_UNSIGNED_INT;
        static const GLenum format = GL_RED_INTEGER;
        static const GLenum read_format = GL_RED;
    };

    template<>
    struct gl_repr<std::array<uint32_t, 2>> {
        static const GLenum internal_format = GL_RG32UI;
        static const GLenum type = GL_UNSIGNED_INT;
        static const GLenum format = GL_RG_INTEGER;
        static const GLenum read_format = GL_RG;
    };

    template<>
    struct gl_repr<depth<float>> {
        static const GLenum internal_format = GL_DEPTH_COMPONENT;
        static const GLenum type = GL_FLOAT;
        static const GLenum format = GL_DEPTH_COMPONENT;
        static const GLenum read_format = GL_DEPTH_COMPONENT;
    };

    enum Wrap {
        Repeat = GL_REPEAT,
        Clamp = GL_CLAMP_TO_EDGE,
        Mirrored = GL_MIRRORED_REPEAT,
    };

    enum Filter {
        Linear = GL_LINEAR,
        Nearest = GL_NEAREST,
    };

    enum Dimensions {
        D1 = GL_TEXTURE_1D,
        D1Array = GL_TEXTURE_1D_ARRAY,
        D2 = GL_TEXTURE_2D,
        D2Array = GL_TEXTURE_2D_ARRAY,
        D3 = GL_TEXTURE_3D,
    };
    template<typename Repr, Dimensions Dim>
    class Texture : public Resource {
    public:
        using repr = gl_repr<Repr>;
        static const GLenum format = repr::type;

        Texture() { glGenTextures(1, &id); }

        Texture(size_t w, size_t h) : Texture() {
            static_assert(Dim == D2);
            glTexImage2D(Dim, 0, repr::internal_format, w, h, 0, repr::format, repr::type, nullptr);
        }

        Texture(Texture &&other) noexcept : id(other.id) { other.id = 0; }

        Texture &operator=(Texture &&other) noexcept {
            id = 0;
            other.id = 0;
            return *this;
        }

        ~Texture() {
            if (id > 0) glDeleteTextures(1, &id);
        }

        void clear_resize(size_t w, size_t h) {
            bind();
            glTexImage2D(Dim, 0, repr::internal_format, w, h, 0, repr::format, repr::type, nullptr);
        }

        template<typename InRepr>
        void upload(std::span<InRepr> data, int w, int h, int mipmap = 0) const {
            typedef gl_repr<InRepr> in_repr;
            static_assert(Dim == D2);
            assert(data.size() == w * h);
            glTexImage2D(Dim, mipmap, repr::internal_format, w, h, 0, in_repr::format, in_repr::type, data);
        }

        void set_wrap(Wrap r, Wrap s, Wrap t) const {
            bind();
            glTexParameteri(Dim, GL_TEXTURE_WRAP_R, r);
            glTexParameteri(Dim, GL_TEXTURE_WRAP_S, s);
            glTexParameteri(Dim, GL_TEXTURE_WRAP_T, t);
        }

        void set_min_mag(Filter min, Filter mag) const {
            bind();
            glTexParameteri(Dim, GL_TEXTURE_MIN_FILTER, min);
            glTexParameteri(Dim, GL_TEXTURE_MAG_FILTER, mag);
        }

        void generate_mipmaps() const { glGenerateMipmap(Dim); }

        Repr read_pixel(size_t x, size_t y) {
            Repr data;
            bind();
            glReadPixels(x, y, 1, 1, repr::read_format, repr::type, &data);
            return data;
        }

        void bind() const { glBindTexture(Dim, id); }

        void bind_to(uint8_t slot) const {
            glActiveTexture(GL_TEXTURE0 + slot);
            bind();
        }

        void unbind() const { glBindTexture(Dim, 0); }

        explicit operator GLuint() const override { return id; }

    private:
        GLuint id{};
    };
}// namespace glow::textures

#endif//IG_PROJET_TEXTURE_H
