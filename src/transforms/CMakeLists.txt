add_library(transforms STATIC Laplace.cpp)
target_link_libraries(transforms PUBLIC OpenMeshCore base PRIVATE glm)