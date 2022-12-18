add_library(base STATIC Mesh.cpp IOException.cpp  ops.cpp )
target_link_libraries(base PUBLIC OpenMeshCore)