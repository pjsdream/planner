#ifndef PLANNER_WINDOW_RENDERER_CONTEXT_H_
#define PLANNER_WINDOW_RENDERER_CONTEXT_H_

#include <unordered_map>

#include <GLFW/glfw3.h>

namespace renderer
{
class RendererBaseWindow;

class RendererContext
{
public:
  static void Init(int& argc, char** argv);
  static void PrintVersion();

  static void RegisterWindow(GLFWwindow* glfw_window, RendererBaseWindow* window);
  static void DeregisterWindow(GLFWwindow* glfw_window);
  static RendererBaseWindow* GetWindow(GLFWwindow* glfw_window);

  static void Start();
  static void Finish();

private:
  // mapping from window id to class pointers, used for callback windows
  static std::unordered_map<GLFWwindow*, RendererBaseWindow*> windows_;

  static bool AreAllWindowsClosed();
};
}

#endif // PLANNER_WINDOW_RENDERER_CONTEXT_H_
