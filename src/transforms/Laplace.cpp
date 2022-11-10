//
// Created by solarliner on 20/10/22.
//

#include "Laplace.h"
#include "../base/ops.h"
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <algorithm>

namespace transforms {
    Laplace::Laplace(V::value_type alpha) : alpha(alpha) {
    }

    void Laplace::smoothen(Mesh &mesh) const {
        mesh.request_vertex_status();
        OpenMesh::VPropHandleT<Mesh::Point> new_pos;
        mesh.add_property(new_pos, "new_pos");

        for (auto v: mesh.vertices()) {
            auto orig = mesh.point(v);
            auto orig_v = V(orig[0], orig[1], orig[2]);
            auto verts = v.vertices().to_vector();
            auto sum = std::transform_reduce(
                    verts.begin(), verts.end(), V(), [](auto a, auto b) { return a + b; }, [&](auto v) {
                        auto p = mesh.point(v);
                        return V(p[0], p[1], p[2]); });
            auto center = sum / (V::value_type) verts.size();
            auto vnew_pos = alpha * orig_v + (1 - alpha) * center;
            mesh.property(new_pos, v) = Mesh::Point(vnew_pos[0], vnew_pos[1], vnew_pos[2]);
        }

        for (auto v: mesh.vertices()) {
            mesh.set_point(v, mesh.property(new_pos, v));
        }

        mesh.release_vertex_status();
    }
}// namespace transforms