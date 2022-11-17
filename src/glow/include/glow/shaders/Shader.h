//
// Created by solarliner on 12/11/22.
//

#ifndef GLOW_SHADER_H
#define GLOW_SHADER_H

#include "../Resource.h"
#include "glad.h"
#include <filesystem>
#include <string>


namespace glow::shaders {
    namespace fs = std::filesystem;
    class Shader : glow::Resource {

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

}// namespace glow::shaders

#endif//GLOW_SHADER_H
