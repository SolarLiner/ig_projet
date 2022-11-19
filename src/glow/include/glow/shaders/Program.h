//
// Created by solarliner on 12/11/22.
//

#ifndef GLOW_PROGRAM_H
#define GLOW_PROGRAM_H

#include "../Resource.h"
#include "../buffers/UniformBuffer.h"
#include "Shader.h"
#include "ShaderException.h"
#include "glm/glm.hpp"

namespace glow::shaders {
    class Program : glow::Resource {
    public:
        Program();
        ~Program();

        void add_shader(const Shader &shader);
        void link() const;
        void use() const;

        template<typename T>
        void set_uniform(const std::string &name, T value) const {
            set_uniform_at(glGetUniformLocation(id, name.c_str()), value);
        }

        void set_uniform_block(const std::string &name, size_t binding) const;

        [[nodiscard]] bool linked() const;
        [[nodiscard]] std::string info_log() const;
        [[nodiscard]] bool valid() const { return has_shaders && linked(); }

        explicit operator GLuint() const override { return id; }

    private:
        void set_uniform_block_at(GLuint bid, size_t binding) const;

        static void set_uniform_at(GLint loc, unsigned int value) ;
        static void set_uniform_at(GLint loc, int value) ;
        static void set_uniform_at(GLint loc, bool value) ;
        static void set_uniform_at(GLint loc, glm::vec2 value) ;
        static void set_uniform_at(GLint loc, glm::vec3 value) ;
        static void set_uniform_at(GLint loc, glm::vec4 value) ;
        static void set_uniform_at(GLint loc, glm::mat4 value) ;

        void ensure() const {
            if (!valid()) throw ShaderException("Program was not ready before it was used");
        }
        GLuint id;
        bool has_shaders = false;
    };
}// namespace glow::shaders


#endif//GLOW_PROGRAM_H
