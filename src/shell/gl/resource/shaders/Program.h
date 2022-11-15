//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_PROGRAM_H
#define IG_PROJET_PROGRAM_H

#include "../buffers/UniformBuffer.h"
#include "../Resource.h"
#include "Shader.h"
#include "ShaderException.h"
#include <glad.h>
#include <span>

namespace shell::gl::resource {
    class Program : Resource {
    public:
        Program();
        ~Program();

        void add_shader(const Shader &shader);
        void link() const;
        void use() const {
            ensure();
            glUseProgram(id);
        }

        template<typename T>
        void set_uniform(const std::string &name, T value) const {
            set_uniform_at(glGetUniformLocation(id, name.c_str()), value);
        }

        void set_uniform_block(const std::string &name, size_t binding) const {
            set_uniform_block_at(glGetUniformBlockIndex(id, name.c_str()), binding);
        }

        [[nodiscard]] bool linked() const;
        [[nodiscard]] std::string info_log() const;
        [[nodiscard]] bool valid() const { return has_shaders && linked(); }

        explicit operator GLuint() const override { return id; }

    private:
        template<typename T>
        void set_uniform_at(GLint, T) const {}

        void set_uniform_block_at(GLuint bid, size_t binding) const {
            glUniformBlockBinding(id, bid, binding);
        }

        template<int dim, typename T, glm::qualifier Q>
        void set_uniform_at(GLint, glm::vec<dim, T, Q>) const {};

        void ensure() const {
            if (!valid()) throw ShaderException("Program was not ready before it was used");
        }
        GLuint id;
        bool has_shaders = false;
    };

    template<>
    inline void Program::set_uniform_at(GLint loc, bool value) const {
        glUniform1i(loc, value);
    }

    template<>
    inline void Program::set_uniform_at(GLint loc, int value) const {
        glUniform1i(loc, value);
    }

    template<>
    inline void Program::set_uniform_at(GLint loc, unsigned int value) const {
        glUniform1ui(loc, value);
    }

    template<>
    inline void Program::set_uniform_at(GLint loc, glm::vec<2, float, glm::defaultp> value) const {
        glUniform2fv(loc, 1, reinterpret_cast<const GLfloat *>(&value));
    }

    template<>
    inline void Program::set_uniform_at(GLint loc, glm::vec<3, float, glm::defaultp> value) const {
        glUniform3fv(loc, 1, reinterpret_cast<const GLfloat *>(&value));
    }

    template<>
    inline void Program::set_uniform_at(GLint loc, glm::vec<4, float, glm::defaultp> value) const {
        glUniform4fv(loc, 1, reinterpret_cast<const GLfloat *>(&value));
    }

    template<>
    inline void Program::set_uniform_at(GLint loc, glm::mat<4, 4, float, glm::defaultp> value) const {
        glUniformMatrix4fv(loc, 1, GL_FALSE, reinterpret_cast<const GLfloat *>(&value));
    }
}// namespace shell::gl::resource


#endif//IG_PROJET_PROGRAM_H
