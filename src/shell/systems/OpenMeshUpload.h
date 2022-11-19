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
        OpenMeshUpload(Shader vs, Shader fs) : program_ref(create_program(vs, fs)) {}
        OpenMeshUpload(const fs::path &vs, const fs::path &fs)
            : OpenMeshUpload(Shader(Shader::Vertex, vs), Shader(Shader::Fragment, fs)) {}
        explicit OpenMeshUpload(const fs::path &shaders_dir)
            : OpenMeshUpload(shaders_dir / "shader.vert", shaders_dir / "shader.frag") {}

        void before_run(Shell &shell) override {
            shell.registry.on_construct<base::Mesh>().connect<&OpenMeshUpload::handle_upload_mesh>(*this);
            shell.registry.on_update<base::Mesh>().connect<&OpenMeshUpload::handle_upload_mesh>(*this);
            shell.registry.on_destroy<base::Mesh>().connect<&OpenMeshUpload::handle_destroy_mesh>();

            auto &camera = shell.resources().get<gl::Camera>();
            camera.bind_to(UniformBindings::Camera);
            program_ref.get_program().set_uniform_block("Camera", UniformBindings::Camera);
            program_ref.get_program().set_uniform_block("Lights", UniformBindings::Lights);

            auto view = shell.registry.view<const base::Mesh>(entt::exclude<gl::Mesh>);
            for (auto entity: view) { handle_upload_mesh(shell.registry, entity); }
        }

    private:
        void handle_upload_mesh(entt::registry &registry, entt::entity entity) {
            spdlog::debug("[OpenMeshUpload] uploading for entity {}", (ENTT_ID_TYPE)entity);
            auto &base_mesh = registry.get<base::Mesh>(entity);
            auto &gl_mesh = registry.get_or_emplace<gl::Mesh>(entity);
            if (!registry.all_of<gl::ProgramRef>(entity)) registry.emplace<gl::ProgramRef>(entity, program_ref);
            gl_mesh.update_from_openmesh(base_mesh);
        }

        static void handle_destroy_mesh(entt::registry &registry, entt::entity entity) {
            spdlog::debug("[OpenMeshUpload] deleting for entity {}", (ENTT_ID_TYPE)entity);
            registry.remove<gl::Mesh>(entity);
            registry.remove<gl::ProgramRef>(entity);
        }

        static Program *create_program(const Shader &vs, const Shader &fs) {
            auto p = new Program();
            p->add_shader(vs);
            p->add_shader(fs);
            p->link();
            return p;
        }

        gl::ProgramRef program_ref;
    };
}// namespace shell::systems

#endif//IG_PROJET_OPENMESHUPLOAD_H
