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

  src/nlp/crf_factor.h
  src/nlp/crf_factor.cc
  src/nlp/crf_node.h
  src/nlp/crf_node.cc
  src/nlp/nlp.h
  src/nlp/nlp.cc

  src/rendering/rendering_window.h
  src/rendering/rendering_window.cc
  src/rendering/camera.h
  src/rendering/camera.cc
  src/resource/raw_mesh_manager.h
  src/resource/raw_mesh.h
  src/resource/raw_mesh_manager.cc
  src/resource/texture.h
  src/resource/texture_manager.h
  src/resource/texture_manager.cc
  src/rendering/light.h
  src/rendering/material.h
  src/rendering/object/array_object.h
  src/rendering/object/array_object.cc
  src/rendering/object/buffer_object.h
  src/rendering/object/buffer_object.cc
  src/rendering/object/texture_object.h
  src/rendering/object/texture_object.cc
  src/rendering/shader/shader.h
  src/rendering/shader/shader.cc
  src/rendering/shader/color_shader.h
  src/rendering/shader/color_shader.cc
  src/rendering/shader/light_shader.h
  src/rendering/shader/light_shader.cc
  src/rendering/shader/pointcloud_shader.h
  src/rendering/shader/pointcloud_shader.cc

  src/robot/robot_joint.h
  src/robot/robot_joint.cc
  src/robot/robot_link.h
  src/robot/robot_link.cc
  src/robot/robot_model.h
  src/robot/robot_model.cc
  src/robot/robot_model_loader.h
  src/robot/robot_model_loader.cc

  src/planning/planner.h
  src/planning/planner.cc

  src/shape/mesh.h
  src/shape/mesh.cc

  # Kinect codes are available only for Windows now.
  #src/vision/kinect.h
  #src/vision/kinect.cc
  )

add_executable(test_rendering
  ${SIMPLAN_SOURCE_FILES}
  tests/test_rendering.cc
  )

target_link_libraries(test_rendering
  Threads::Threads
  ${ASSIMP_LIBRARIES}
  ${OPENGL_gl_LIBRARY}
  glfw)
