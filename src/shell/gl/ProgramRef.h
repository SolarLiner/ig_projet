//
// Created by solarliner on 14/11/22.
//

#ifndef IG_PROJET_PROGRAMREF_H
#define IG_PROJET_PROGRAMREF_H

#include "resource/shaders/Program.h"
#include <memory>

namespace shell {
    class ProgramRef {
    public:
        explicit ProgramRef(gl::resource::Program *program): program(program) {}
        ProgramRef(ProgramRef &ref) = default;

        [[nodiscard]] const gl::resource::Program &get_program() const {
            return *program;
        }

    private:
        std::shared_ptr<gl::resource::Program> program;
    };
}// namespace shell

#endif//IG_PROJET_PROGRAMREF_H
