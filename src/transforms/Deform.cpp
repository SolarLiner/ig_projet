//
// Created by solarliner on 03/12/22.
//

#include "Deform.h"

namespace transforms {
    void Deform::operator()(Mesh &mesh, Mesh::VertexHandle handle) const {
        OpenMesh::VPropHandleT<float> weight_prop;
        mesh.add_property(weight_prop, "deformation weight");
        calculate_weights(mesh, handle, weight_prop);

        mesh.request_vertex_status();
        for (auto v: mesh.vertices()) {
            auto point = mesh.point(v);
            vec3 pt(point[0], point[1], point[2]);
            auto k = mesh.property(weight_prop, v);
            auto tgt_vec = pt + dir * k;
            mesh.set_point(v, {tgt_vec[0], tgt_vec[1], tgt_vec[2]});
        }
        mesh.release_vertex_status();
        mesh.remove_property(weight_prop);
    }


    void Deform::calculate_weights(Mesh &mesh, OpenMesh::VertexHandle handle,
                                   const OpenMesh::VPropHandleT<float> &weight_prop) const {
        auto point = mesh.point(handle);
        vec3 handle_vec(point[0], point[1], point[2]);

        mesh.request_vertex_status();
        for (auto v: mesh.vertices()) {
            auto vpt = mesh.point(v);
            vec3 vec(vpt[0], vpt[1], vpt[2]);
            auto f = glm::distance(handle_vec, vec);
            auto k = std::clamp(max_distance - f, 0.f, max_distance) / max_distance;
            mesh.property(weight_prop, v) = k;
        }
        mesh.release_vertex_status();
    }

    void Deform::preview_weight_in_color(Mesh &mesh, Mesh::VertexHandle handle) const {
        OpenMesh::VPropHandleT<float> weight_prop;
        mesh.add_property(weight_prop, "deformation weight");
        calculate_weights(mesh, handle, weight_prop);

        mesh.request_vertex_status();
        mesh.request_vertex_colors();
        for (auto v: mesh.vertices()) {
            auto k = mesh.property(weight_prop, v);
            mesh.set_color(v, {255.f * k, 255.f * k, 255.f * k});
        }
        mesh.release_vertex_colors();
        mesh.release_vertex_status();

        mesh.remove_property(weight_prop);
    }
}