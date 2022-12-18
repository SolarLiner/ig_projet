add_library(transforms STATIC Laplace.cpp Deform.cpp Deform.h)
target_link_libraries(transforms PUBLIC OpenMeshCore base PRIVATE glm)