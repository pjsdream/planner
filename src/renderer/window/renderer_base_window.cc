#include "renderer/window/renderer_base_window.h"
#include "renderer/window/renderer_context.h"

#include <iostream>

namespace renderer
{
void RendererBaseWindow::RefreshCallback(GLFWwindow* window)
{
  RendererContext::GetWindow(window)->Display();
}

void RendererBaseWindow::ReshapeCallback(GLFWwindow* window, int width, int height)
{
  RendererContext::GetWindow(window)->Reshape(width, height);
}

void RendererBaseWindow::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
  int interpreted_button =
      button == GLFW_MOUSE_BUTTON_LEFT ? 0 :
      button == GLFW_MOUSE_BUTTON_MIDDLE ? 1 : 2;

  int interpreted_state =
      action == GLFW_PRESS ? 1 : 0;

  double x, y;
  glfwGetCursorPos(window, &x, &y);

  RendererContext::GetWindow(window)->Mouse(interpreted_button, interpreted_state, x, y);
}

void RendererBaseWindow::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS ||
      glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS ||
      glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    RendererContext::GetWindow(window)->Motion(xpos, ypos);

  else
    RendererContext::GetWindow(window)->PassiveMotion(xpos, ypos);
}

RendererBaseWindow::RendererBaseWindow()
{
  // Create and register window
  window_ = glfwCreateWindow(1024, 768, "renderer base window", nullptr, nullptr);
  glfwSetWindowPos(window_, 200, 200);

  // Make the context manager know a window is created
  RendererContext::RegisterWindow(window_, this);
  
  // Register callbacks
  glfwSetWindowRefreshCallback(window_, RefreshCallback);
  glfwSetWindowSizeCallback(window_, ReshapeCallback);
  glfwSetMouseButtonCallback(window_, MouseButtonCallback);
  glfwSetCursorPosCallback(window_, CursorPosCallback);

  // Make current for following OpenGL initializations in constructors of derived classes
  MakeCurrent();

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
  }
}

RendererBaseWindow::~RendererBaseWindow()
{
  RendererContext::DeregisterWindow(window_);
  glfwDestroyWindow(window_);
}

void RendererBaseWindow::MakeCurrent()
{
  glfwMakeContextCurrent(window_);
}

bool RendererBaseWindow::ShouldClose()
{
  return static_cast<bool>(glfwWindowShouldClose(window_));
}

void RendererBaseWindow::ChangeWindowTitle(const std::string& title)
{
  glfwSetWindowTitle(window_, title.c_str());
}

void RendererBaseWindow::SwapBuffers()
{
  glfwSwapBuffers(window_);
}

void RendererBaseWindow::Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glfwSwapBuffers(window_);
}

void RendererBaseWindow::Reshape(int width, int height)
{
  glViewport(0, 0, width, height);
}

void RendererBaseWindow::Mouse(int button, int state, double x, double y)
{
}

void RendererBaseWindow::Motion(double x, double y)
{
}

void RendererBaseWindow::PassiveMotion(double x, double y)
{
}

void RendererBaseWindow::Idle()
{
}
}
