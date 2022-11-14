add_library(transforms STATIC Laplace.cpp Laplace.h)
target_link_libraries(transforms PUBLIC OpenMeshCore base)