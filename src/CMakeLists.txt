add_subdirectory(base)
add_subdirectory(glow)
add_subdirectory(shell)
add_subdirectory(transforms)

add_executable(ig_projet main.cpp)
target_link_libraries(ig_projet PUBLIC base shell transforms PRIVATE spdlog::spdlog)
