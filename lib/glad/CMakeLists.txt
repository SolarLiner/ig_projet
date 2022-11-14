find_package(OpenGL REQUIRED)

add_library(glad SHARED glad.c)
target_link_libraries(glad PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${OPENGL_INCLUDE_DIRECTORIES})