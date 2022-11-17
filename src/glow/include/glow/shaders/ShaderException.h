//
// Created by solarliner on 12/11/22.
//

#ifndef GLOW_SHADEREXCEPTION_H
#define GLOW_SHADEREXCEPTION_H

#include <exception>
#include <string>

namespace glow::shaders {
    class ShaderException : public std::exception {
    public:
        explicit ShaderException(std::string message) : message(std::move(message)) {}

        [[nodiscard]] const char *what() const noexcept override { return message.c_str(); }

    private:
        std::string message;
    };
}// namespace glow::shaders

#endif//GLOW_SHADEREXCEPTION_H
