cmake_minimum_required(VERSION 3.1.0)
project(simplan)

# C++17
set(CMAKE_CXX_STANDARD 17)

# C++ thread
find_package(Threads)

# fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# eigen
find_package(Eigen3 REQUIRED)

# assimp
find_package(assimp REQUIRED)

# glfw
find_package(glfw3 3.2 REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

include_directories(
  src
  lib
  ${EIGEN3_INCLUDE_DIRS}
  ${ASSIMP_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
)

set(SIMPLAN_SOURCE_FILES
  # GLAD
  lib/glad/glad.h
  lib/glad/glad.c

  # lodepng
  lib/lodepng/lodepng.h
  lib/lodepng/lodepng.cpp

  src/rendering/rendering_window.h
  src/rendering/rendering_window.cc
  src/planning/planner.h
  src/planning/planner.cc
  src/rendering/camera.h
  src/rendering/camera.cc src/shape/mesh.cc src/shape/mesh.h src/resource/raw_mesh_manager.h src/resource/raw_mesh.h src/resource/raw_mesh_manager.cc src/resource/texture.h src/resource/texture_manager.h src/resource/texture_manager.cc src/rendering/light.h src/rendering/shader/shader.h src/rendering/shader/shader.cc src/rendering/shader/color_shader.h src/rendering/shader/color_shader.cc src/rendering/shader/light_shader.h src/rendering/shader/light_shader.cc)

add_executable(test_rendering
  ${SIMPLAN_SOURCE_FILES}
  tests/test_rendering.cc
  )

target_link_libraries(test_rendering
  Threads::Threads
  ${ASSIMP_LIBRARIES}
  ${OPENGL_gl_LIBRARY}
  glfw)
