//
// Created by solarliner on 12/11/22.
//

#include "glow/shaders/Program.h"
#include <glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace glow::shaders {
    Program::Program() : glow::Resource(), id(glCreateProgram()) {}

    Program::~Program() { glDeleteProgram(id); }

    void Program::add_shader(const Shader &shader) {
        assert(shader.valid());
        glAttachShader(id, GLuint(shader));
        has_shaders = true;
    }

    void Program::link() const {
        glLinkProgram(id);
        if (!linked()) { throw ShaderException(info_log()); }
    }

    bool Program::linked() const {
        GLint status;
        glGetProgramiv(id, GL_LINK_STATUS, &status);
        return status == GL_TRUE;
    }

    std::string Program::info_log() const {
        GLint length;
        std::string log;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
        log.resize(length);
        glGetProgramInfoLog(id, length, &length, log.data());
        log.resize(length);
        return log;
    }
    void Program::set_uniform_block_at(GLuint bid, size_t binding) const { glUniformBlockBinding(id, bid, binding); }
    void Program::set_uniform_block(const std::string &name, size_t binding) const {
        set_uniform_block_at(glGetUniformBlockIndex(id, name.c_str()), binding);
    }
    void Program::use() const {
        ensure();
        glUseProgram(id);
    }

    void Program::set_uniform_at(GLint loc, bool value) { glUniform1i(loc, value); }

    void Program::set_uniform_at(GLint loc, int value) { glUniform1i(loc, value); }

    void Program::set_uniform_at(GLint loc, unsigned int value) { glUniform1ui(loc, value); }

    void Program::set_uniform_at(GLint loc, glm::vec2 value) { glUniform2fv(loc, 1, glm::value_ptr(value)); }

    void Program::set_uniform_at(GLint loc, glm::vec3 value) { glUniform3fv(loc, 1, glm::value_ptr(value)); }

    void Program::set_uniform_at(GLint loc, glm::vec4 value) { glUniform4fv(loc, 1, glm::value_ptr(value)); }

    void Program::set_uniform_at(GLint loc, glm::mat4 value) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }
}// namespace glow::shaders
