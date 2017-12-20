#include "renderer/window/renderer_context.h"

#include <iostream>
#include <thread>

#include "renderer/window/renderer_base_window.h"
#include "renderer/window/renderer_window.h"

namespace renderer
{
// mapping from window id to class pointers, used for callback windows
std::unordered_map<GLFWwindow*, RendererBaseWindow*> RendererContext::windows_;

void RendererContext::RegisterWindow(GLFWwindow* glfw_window, RendererBaseWindow* window)
{
  windows_[glfw_window] = window;
}

void RendererContext::DeregisterWindow(GLFWwindow* glfw_window)
{
  windows_.erase(glfw_window);
}

RendererBaseWindow* RendererContext::GetWindow(GLFWwindow* glfw_window)
{
  return windows_[glfw_window];
}

void RendererContext::Init(int& argc, char** argv)
{
  glfwInit();

  // Set OpenGL version to 4.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void RendererContext::PrintVersion()
{
  // TODO: glfw version

  const auto opengl_version = glGetString(GL_VERSION);
  std::cout << "OpenGL Version: " << opengl_version << "\n";
}

bool RendererContext::AreAllWindowsClosed()
{
  for (auto& window : windows_)
    if (!window.second->ShouldClose())
      return false;

  return true;
}

void RendererContext::Start()
{
  // Call reshape function at the beginning
  for (auto& window_pair : windows_)
  {
    auto& glfw_window = window_pair.first;
    auto& window = window_pair.second;

    int width, height;
    glfwGetWindowSize(glfw_window, &width, &height);

    window->MakeCurrent();
    window->Reshape(width, height);
  }

  // GLFW event loop
  while (!AreAllWindowsClosed())
  {
    for (auto& window_pair : windows_)
    {
      auto& window = window_pair.second;

      if (window->ShouldClose())
        break;

      window->MakeCurrent();
      window->Idle();

      glfwPollEvents();
    }
  }
}

void RendererContext::Finish()
{
  glfwTerminate();
}
}
