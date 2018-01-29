#ifndef RENDERER_WIDGET_WIDGET_H_
#define RENDERER_WIDGET_WIDGET_H_

#include <memory>
#include <vector>

namespace renderer
{
class Widget
{
public:
  Widget();

  auto Width() const
  {
    return width_;
  }

  void SetWidth(int width)
  {
    width_ = width;
  }

  void SetHeight(int height)
  {
    height_ = height;
  }

  auto Height() const
  {
    return height_;
  }

  void AddWidget(int row, int col, const std::shared_ptr<Widget>& widget);

  virtual void Mouse(int button, int state, double x, double y);
  virtual void Motion(int button, int state, double x, double y);
  virtual void Idle();

private:
  std::weak_ptr<Widget> parent_widget_;
  std::vector<std::vector<std::shared_ptr<Widget>>> widgets_;

  int x_ = 0;
  int y_ = 0;
  int width_ = 10;
  int height_ = 10;

  int num_rows_ = 0;
  int num_cols_ = 0;
};
}

#endif // RENDERER_WIDGET_WIDGET_H_
