//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_SHADER_H
#define IG_PROJET_SHADER_H

#include "../Resource.h"
#include "glm/detail/type_mat4x4.hpp"
#include <filesystem>
#include <functional>
#include <glad.h>
#include <glm/common.hpp>
#include <string>

namespace fs = std::filesystem;

namespace shell::gl::resource {
    class Shader : Resource {
    public:
        enum Type {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
        };

        explicit Shader(Type type);
        Shader(Type type, const fs::path &path);
        Shader(Type type, const std::string &glsl);
        ~Shader();

        void compile(const std::string &glsl);

        [[nodiscard]] bool valid() const { return initialized && compiled(); }

        [[nodiscard]] bool compiled() const;
        [[nodiscard]] std::string info_log() const;

        explicit operator GLuint() const override { return id; }

    private:

        GLuint id;
        bool initialized = false;
    };

}// namespace shell::gl::resource

#endif//IG_PROJET_SHADER_H
