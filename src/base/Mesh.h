//
// Created by solarliner on 20/10/22.
//

#ifndef IG_PROJET_MESH_H
#define IG_PROJET_MESH_H

#include "polyscope/surface_mesh.h"
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <glm/glm.hpp>

namespace base {
    typedef glm::vec<3, double> V;
    typedef std::array<unsigned int, 3> F;

    class Traits: public OpenMesh::DefaultTraits {
    public:
//        using Point = V;
//        using Normal = V;
        VertexAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Normal);
        FaceAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Color | OpenMesh::Attributes::Normal);
        EdgeAttributes(OpenMesh::Attributes::Status);
    };

    class Mesh: public OpenMesh::TriMesh_ArrayKernelT<Traits> {
    public:
        [[nodiscard]] static Mesh open(std::string filename);
        void save(const std::string& filename) const;
        [[nodiscard]] polyscope::SurfaceMesh *show(std::string name) const;

    private:
        [[nodiscard]] std::vector<glm::vec3> ps_vertices() const;
        [[nodiscard]] std::vector<F> ps_faces() const;
    };
}


#endif//IG_PROJET_MESH_H
