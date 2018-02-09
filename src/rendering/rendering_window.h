#ifndef SIMPLAN_RENDERING_RENDERING_WINDOW_H_
#define SIMPLAN_RENDERING_RENDERING_WINDOW_H_

#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rendering/camera.h"

namespace simplan
{
class RenderingWindow
{
public:
  RenderingWindow();

  bool ShouldClose() const;

  void Run();

  void MouseButton(int button, int action, int mods, double xpos, double ypos);
  void CursorPos(double xpos, double ypos);
  void Resize(int width, int height);

private:
  void LoadShaders();
  GLuint LoadShaderFromFile(const std::string& filename, GLenum type);
  GLuint LinkShaders(const std::vector<GLuint>& shaders);

  GLFWwindow* window_;

  std::shared_ptr<Camera> camera_;

  GLuint light_program_;
  GLuint color_program_;
  GLuint texture_program_;

  bool mouse_button_pressed_[2];
  double last_mouse_xpos_;
  double last_mouse_ypos_;

  int width_;
  int height_;
};
}

#endif // SIMPLAN_RENDERING_RENDERING_WINDOW_H_
