//
// Created by solarliner on 20/11/22.
//

#ifndef IG_PROJET_TEXTURE_H
#define IG_PROJET_TEXTURE_H

#include "glm/vec4.hpp"
#include "glow/Resource.h"

#include <span>

namespace glow::textures {
    template<typename T>
    struct depth {};

    template<typename Repr>
    struct format_name {
//        static_assert(false, "Repr not supported");
        static const GLenum kind;
        static const GLenum name;
    };

    template<>
    struct format_name<glm::vec4> {
        static const GLenum kind = GL_RGBA;
        static const GLenum name = GL_RGBA32F;
    };

    template<>
    struct format_name<unsigned int> {
        static const GLenum kind = GL_RED;
        static const GLenum name = GL_UNSIGNED_INT;
    };

    template<>
    struct format_name<depth<float>> {
        static const GLenum kind = GL_DEPTH;
        static const GLenum name = GL_FLOAT;
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
        using repr = format_name<Repr>;
        static const GLenum format = repr::name;

        Texture() { glGenTextures(1, &id); }

        Texture(size_t w, size_t h) : Texture() {
            static_assert(Dim == D2);
            glTexImage2D(Dim, 0, repr::kind, w, h, 0, repr::kind, repr::name, nullptr);
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

        void upload(std::span<Repr> data, int w, int h, int mipmap = 0) const {
            static_assert(Dim == D2);
            assert(data.size() == w * h);
            glTexImage2D(Dim, mipmap, repr::kind, w, h, 0, repr::kind, repr::name, data);
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
            glReadPixels(x, y, 1, 1, repr::kind, repr::name, &data);
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
