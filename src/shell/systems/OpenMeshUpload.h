//
// Created by solarliner on 14/11/22.
//

#ifndef IG_PROJET_OPENMESHUPLOAD_H
#define IG_PROJET_OPENMESHUPLOAD_H

#include "../../base/Mesh.h"
#include "../UniformBindings.h"
#include "../gl/Mesh.h"
#include "../gl/ProgramRef.h"
#include "../gl/resource/shaders/Program.h"
#include "../gl/resource/shaders/Shader.h"
#include "System.h"
#include <filesystem>

namespace shell {
    using namespace gl::resource;
    namespace systems {
        namespace fs = std::filesystem;
        class OpenMeshUpload : public System {
        public:
            OpenMeshUpload(Shader vs, Shader fs) : program_ref(create_program(vs, fs)) {}
            OpenMeshUpload(const fs::path &vs, const fs::path &fs)
                : OpenMeshUpload(Shader(gl::resource::Shader::Vertex, vs), Shader(gl::resource::Shader::Fragment, fs)) {
            }
            explicit OpenMeshUpload(const fs::path &shaders_dir)
                : OpenMeshUpload(shaders_dir / "shader.vert", shaders_dir / "shader.frag") {}

            void before_run(const sf::Window &window, entt::registry &registry) override {
                registry.on_construct<base::Mesh>().connect<&OpenMeshUpload::handle_upload_mesh>(*this);
                registry.on_update<base::Mesh>().connect<&OpenMeshUpload::handle_upload_mesh>(*this);
                registry.on_destroy<base::Mesh>().connect<&OpenMeshUpload::handle_destroy_mesh>();

                auto &camera = registry.ctx().get<gl::Camera>();
                camera.bind_to(UniformBindings::Camera);
                program_ref.get_program().set_uniform_block("Camera", UniformBindings::Camera);
                program_ref.get_program().set_uniform_block("Lights", UniformBindings::Lights);

                auto view = registry.view<const base::Mesh>(entt::exclude<gl::Mesh>);
                for (auto entity: view) { handle_upload_mesh(registry, entity); }
            }

        private:
            void handle_upload_mesh(entt::registry &registry, entt::entity entity) {
                auto &base_mesh = registry.get<base::Mesh>(entity);
                auto &gl_mesh = registry.get_or_emplace<gl::Mesh>(entity);
                if (!registry.all_of<ProgramRef>(entity)) registry.emplace<ProgramRef>(entity, program_ref);
                gl_mesh.update_from_openmesh(base_mesh);
            }

            static void handle_destroy_mesh(entt::registry &registry, entt::entity entity) {
                registry.remove<gl::Mesh>(entity);
                registry.remove<ProgramRef>(entity);
            }

            static Program *create_program(const Shader &vs, const Shader &fs) {
                auto p = new Program();
                p->add_shader(vs);
                p->add_shader(fs);
                p->link();
                return p;
            }

            ProgramRef program_ref;
        };
    }// namespace systems
}// namespace shell

#endif//IG_PROJET_OPENMESHUPLOAD_H
