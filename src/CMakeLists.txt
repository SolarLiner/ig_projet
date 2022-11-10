add_subdirectory(base)
add_subdirectory(transforms)

add_executable(ig_projet main.cpp)
target_link_libraries(ig_projet OpenMeshCore polyscope base transforms)
