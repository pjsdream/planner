cmake_minimum_required(VERSION 3.1.0)
project(planner)

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
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Use OpenGL4 by defining GL_GLEXT_PROTOTYPES throughout the project
add_definitions(-DGL_GLEXT_PROTOTYPES)

# cppzmq
find_package(cppzmq REQUIRED)
message(${cppzmq_INCLUDE_DIR})

include_directories(
  src
  lib
  ${EIGEN3_INCLUDE_DIRS}
  ${GLFW_LIBRARIES}
  ${ASSIMP_INCLUDE_DIRS}
)

add_library(renderer
  # lodepng
  lib/lodepng/lodepng.h
  lib/lodepng/lodepng.cpp

  src/renderer/window/renderer_base_window.h
  src/renderer/window/renderer_base_window.cc
  src/renderer/light/light.h
  src/renderer/material/material.h
  src/renderer/gl/resource/gl_object.h
  src/renderer/gl/resource/gl_object.cc
  src/renderer/gl/resource/gl_texture.h
  src/renderer/gl/resource/gl_texture.cc
  src/renderer/gl/shader/shader.h
  src/renderer/gl/shader/shader.cc
  src/renderer/camera/camera.h
  src/renderer/camera/camera.cc
  src/renderer/window/renderer_context.h
  src/renderer/window/renderer_context.cc
  src/renderer/window/renderer_window.h
  src/renderer/window/renderer_window.cc
  src/renderer/gl/shader/light_shader.h
  src/renderer/gl/shader/light_shader.cc
  src/renderer/scene/scene.h
  src/renderer/scene/scene.cc
  src/renderer/scene/scene_node.h
  src/renderer/scene/scene_node.cc
  src/renderer/scene/scene_object.h
  src/renderer/gl/resource/gl_instances.h
  src/renderer/gl/resource/gl_instances.cc
  src/renderer/gl/resource/gl_object_manager.h)

target_link_libraries(renderer
  Threads::Threads
  stdc++fs
  ${cppzmq_LIBRARY}
  ${ASSIMP_LIBRARIES}
  ${OPENGL_LIBRARIES}
  glfw
  ${GLFW_LIBRARIES}
)

add_executable(test_renderer
  src/renderer/test/main.cc
  )
target_link_libraries(test_renderer renderer)

add_executable(planner
  # tinyxml2
  lib/tinyxml2/tinyxml2.h
  lib/tinyxml2/tinyxml2.cpp

  src/planner/planner_window.h
  src/planner/planner_window.cc
  src/planner/main.cc
  src/json/json.h
  src/json/json.cc
  src/json/json_loader.h
  src/json/json_loader.cc
  src/robot/robot_link.h src/robot/robot_joint.h src/robot/robot_joint.cc src/robot/robot_model.h src/robot/robot_model_loader.h src/robot/robot_model_loader.cc src/robot/robot_model.cc src/planner/rendering/robot_scene.h src/planner/rendering/robot_scene.cc src/planner/planner.h src/planner/planner.cc src/planner/environment/environment.h src/planner/environment/environment_loader.h src/planner/environment/environment_loader.cc src/planner/environment/object.h src/shape/shape.h src/shape/cube.h)
target_link_libraries(planner renderer)