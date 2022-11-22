//
// Created by solarliner on 21/11/22.
//

#include "OpenMeshUpload.h"
#include "../Shell.h"

namespace shell::systems {

    OpenMeshUpload::OpenMeshUpload(Shader vs, Shader fs) : program_ref(create_program(vs, fs)) {}
    OpenMeshUpload::OpenMeshUpload(const fs::path &vs, const fs::path &fs)
            : OpenMeshUpload(Shader(Shader::Vertex, vs), Shader(Shader::Fragment, fs)) {}
    OpenMeshUpload::OpenMeshUpload(const fs::path &shaders_dir)
            : OpenMeshUpload(shaders_dir / "shader.vert", shaders_dir / "shader.frag") {}
    void OpenMeshUpload::before_run(Shell &shell) {
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
    void OpenMeshUpload::handle_upload_mesh(entt::registry &registry, entt::entity entity) {
        spdlog::debug("[OpenMeshUpload] uploading for entity {}", (ENTT_ID_TYPE)entity);
        auto &base_mesh = registry.get<base::Mesh>(entity);
        auto &gl_mesh = registry.get_or_emplace<gl::Mesh>(entity);
        if (!registry.all_of<gl::ProgramRef>(entity)) registry.emplace<gl::ProgramRef>(entity, program_ref);
        gl_mesh.update_from_openmesh(base_mesh);
    }
    void OpenMeshUpload::handle_destroy_mesh(entt::registry &registry, entt::entity entity) {
        spdlog::debug("[OpenMeshUpload] deleting for entity {}", (ENTT_ID_TYPE)entity);
        registry.remove<gl::Mesh>(entity);
        registry.remove<gl::ProgramRef>(entity);
    }
    Program *OpenMeshUpload::create_program(const Shader &vs, const Shader &fs) {
        auto p = new Program();
        p->add_shader(vs);
        p->add_shader(fs);
        p->link();
        return p;
    }
}