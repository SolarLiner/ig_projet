//
// Created by solarliner on 12/11/22.
//

#include "Program.h"
#include "ShaderException.h"

namespace shell::gl::resource {
    Program::Program(): Resource(), id(glCreateProgram()) {
    }

    Program::~Program() {
        glDeleteProgram(id);
    }

    void Program::add_shader(const Shader &shader) {
        assert(shader.valid());
        glAttachShader(id, GLuint(shader));
        has_shaders = true;
    }

    void Program::link() const {
        glLinkProgram(id);
        if(!linked()) {
            throw ShaderException(info_log());
        }
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
}
