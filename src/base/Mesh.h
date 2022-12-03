//
// Created by solarliner on 20/10/22.
//

#ifndef IG_PROJET_BASE_MESH_H
#define IG_PROJET_BASE_MESH_H

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

namespace base {
    class Traits : public OpenMesh::DefaultTraits {
    public:
//        using Point = V;
//        using Normal = V;
        VertexAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color |
                         OpenMesh::Attributes::TexCoord2D);
        FaceAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Normal);
        EdgeAttributes(OpenMesh::Attributes::Status);
    };

    class Mesh : public OpenMesh::TriMesh_ArrayKernelT<Traits> {
    public:
        static constexpr bool in_place_delete = true;
        [[nodiscard]] static Mesh open(const std::string &filename);

        void save(const std::string &filename) const;

        void reset_vertex_colors();
    };
}


#endif//IG_PROJET_BASE_MESH_H
