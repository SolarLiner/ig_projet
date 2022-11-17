add_library(glow Shader.cpp Program.cpp)

target_include_directories(glow PUBLIC include)

target_link_libraries(glow PUBLIC glm glad)