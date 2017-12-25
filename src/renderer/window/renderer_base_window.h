#ifndef PLANNER_WINDOW_RENDERER_BASE_WINDOW_H_
#define PLANNER_WINDOW_RENDERER_BASE_WINDOW_H_

#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace renderer
{
class RendererBaseWindow
{
  friend class RendererContext;

private:
  static void RefreshCallback(GLFWwindow* window);
  static void ReshapeCallback(GLFWwindow* window, int width, int height);
  static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
  static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

public:
  RendererBaseWindow();
  ~RendererBaseWindow();

  void ChangeWindowTitle(const std::string& title);

  void MakeCurrent();
  bool ShouldClose();

protected:
  virtual void Display();
  virtual void Reshape(int width, int height);
  virtual void Mouse(int button, int state, double x, double y);
  virtual void Motion(double x, double y);
  virtual void PassiveMotion(double x, double y);
  virtual void Idle();

  void SwapBuffers();

private:
  GLFWwindow* window_;
};
}

#endif // PLANNER_WINDOW_RENDERER_BASE_WINDOW_H_
