//
// Created by solarliner on 14/11/22.
//

#ifndef IG_PROJET_OPENMESHUPLOAD_H
#define IG_PROJET_OPENMESHUPLOAD_H

#include "../../base/Mesh.h"
#include "../UniformBindings.h"
#include "../gl/Mesh.h"
#include "../gl/ProgramRef.h"
#include "System.h"
#include <filesystem>
#include <glow/shaders/Shader.h>

namespace shell::systems {
    using namespace glow::shaders;
    namespace fs = std::filesystem;
    class OpenMeshUpload : public System {
    public:
        OpenMeshUpload(Shader vs, Shader fs);
        OpenMeshUpload(const fs::path &vs, const fs::path &fs);
        explicit OpenMeshUpload(const fs::path &shaders_dir);

        void before_run(Shell &shell) override;

    private:
        void handle_upload_mesh(entt::registry &registry, entt::entity entity);

        static void handle_destroy_mesh(entt::registry &registry, entt::entity entity);

        static Program *create_program(const Shader &vs, const Shader &fs);

        gl::ProgramRef program_ref;
    };
}// namespace shell::systems

#endif//IG_PROJET_OPENMESHUPLOAD_H
