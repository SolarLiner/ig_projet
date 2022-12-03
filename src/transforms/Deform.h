//
// Created by solarliner on 03/12/22.
//

#ifndef IG_PROJET_DEFORM_H
#define IG_PROJET_DEFORM_H

#include <glm/glm.hpp>
#include "../base/Mesh.h"

namespace transforms {
    using glm::vec3;
    using base::Mesh;
    class Deform {
    public:
        vec3 dir;
        float max_distance;

        void operator()(Mesh &mesh, Mesh::VertexHandle handle) const;

        void preview_weight_in_color(Mesh &mesh, Mesh::VertexHandle handle) const;

        void calculate_weights(Mesh &mesh, Mesh::VertexHandle handle,
                               const OpenMesh::VPropHandleT<float> &weight_prop) const;
    };
}


#endif //IG_PROJET_DEFORM_H
