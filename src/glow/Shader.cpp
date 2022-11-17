//
// Created by solarliner on 12/11/22.
//

#include "include/glow/shaders/Shader.h"
#include "include/glow/shaders/ShaderException.h"
#include <fstream>

namespace glow::shaders {
    Shader::Shader(Shader::Type type) : id(glCreateShader(type)) {}

    Shader::Shader(Shader::Type type, const std::string &glsl) : Shader(type) { compile(glsl); }

    Shader::Shader(Shader::Type type, const fs::path &path) : Shader(type) {
        std::ifstream file(path);
        std::stringstream data;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        data << file.rdbuf();
        compile(data.str());
        file.close();
    }

    Shader::~Shader() { glDeleteShader(id); }

    void Shader::compile(const std::string &glsl) {
        const char *glsl_addr = glsl.c_str();
        glShaderSource(id, 1, &glsl_addr, nullptr);
        glCompileShader(id);
        if (!compiled()) { throw ShaderException(info_log()); }
        initialized = true;
    }

    bool Shader::compiled() const {
        GLint status;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);
        return status == GL_TRUE;
    }

    std::string Shader::info_log() const {
        std::string log;
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        log.resize(length);
        glGetShaderInfoLog(id, length, &length, log.data());
        log.resize(length);
        return log;
    }
}// namespace glow::shaders