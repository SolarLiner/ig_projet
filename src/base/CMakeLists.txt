add_library(base Mesh.cpp IOException.cpp IOException.h ops.cpp ops.h)
target_link_libraries(base OpenMeshCore glm polyscope)