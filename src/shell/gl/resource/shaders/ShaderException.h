//
// Created by solarliner on 12/11/22.
//

#ifndef IG_PROJET_SHADEREXCEPTION_H
#define IG_PROJET_SHADEREXCEPTION_H

#include <exception>
#include <string>
#include <utility>

namespace shell {
    namespace gl {
        namespace resource {
            class ShaderException: public std::exception {
            public:
                ShaderException(std::string message): message(std::move(message)) {}

                [[nodiscard]] const char * what() const noexcept override {
                    return message.c_str();
                }
            private:
                std::string message;
            };

        }// namespace resource
    }    // namespace gl
}// namespace shell

#endif//IG_PROJET_SHADEREXCEPTION_H
