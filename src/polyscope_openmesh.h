#include "OpenMesh/Core/Mesh/Attributes.hh"
#include "OpenMesh/Core/Mesh/Traits.hh"
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/IO/importer/ImporterT.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <array>
#include <polyscope/polyscope.h>
#include <polyscope/surface_mesh.h>
#include <vector>

struct PolyscopeTraits : public OpenMesh::DefaultTraits {
    VertexAttributes(OpenMesh::Attributes::Status);
    FaceAttributes(OpenMesh::Attributes::Status | OpenMesh::Attributes::Color);
    EdgeAttributes(OpenMesh::Attributes::Status);
};

struct PolyscopeMesh : public OpenMesh::TriMesh_ArrayKernelT<PolyscopeTraits> {
    static PolyscopeMesh read_mesh(const std::string &filename);
    [[nodiscard]] std::vector<Point> ps_vertices() const;
    [[nodiscard]] std::vector<std::array<unsigned int, 3>> ps_faces() const;
    void save_mesh(const std::string &filename) const;
};