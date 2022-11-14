add_library(base STATIC Mesh.cpp IOException.cpp IOException.h ops.cpp ops.h)
target_link_libraries(base PUBLIC OpenMeshCore)