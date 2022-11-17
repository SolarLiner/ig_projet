//
// Created by solarliner on 14/11/22.
//

#ifndef IG_PROJET_PROGRAMREF_H
#define IG_PROJET_PROGRAMREF_H

#include <glow/shaders/Program.h>
#include <memory>

namespace shell::gl {
    using namespace glow::shaders;
    class ProgramRef {
    public:
        explicit ProgramRef(Program *program): program(program) {}
        ProgramRef(ProgramRef &ref) = default;

        [[nodiscard]] const Program &get_program() const {
            return *program;
        }

    private:
        std::shared_ptr<Program> program;
    };
}// namespace shell

#endif//IG_PROJET_PROGRAMREF_H
