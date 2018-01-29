#ifndef RENDERER_WINDOW_WIDGET_WINDOW_H_
#define RENDERER_WINDOW_WIDGET_WINDOW_H_

namespace renderer
{
class WidgetWindow : public RendererBaseWindow
{
public:
  WidgetWindow();

  ~WidgetWindow() = default;

protected:
  void Display() override;
  void Reshape(int width, int height) override;
  void Mouse(int button, int state, double x, double y) override;
  void Motion(double x, double y) override;

private:
  int last_mouse_button_;
  int last_mouse_state;
  int last_mouse_x_;
  int last_mouse_y_;
};
}

#endif // RENDERER_WINDOW_WIDGET_WINDOW_H_