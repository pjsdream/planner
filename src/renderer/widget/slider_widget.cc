#include "renderer/widget/slider_widget.h"

namespace renderer
{
SliderWidget::SliderWidget(double min_value, double max_value, double default_value)
  : Widget(), min_value_(min_value), max_value_(max_value), value_(default_value)
{
}

void SliderWidget::Mouse(int button, int state, double x, double y)
{
  if (button == 0 && state == 1)
  {
    const double t = static_cast<double>(x) / Width();
    value_ = (1. - t) * min_value_ + t * max_value_;
  }
}

void SliderWidget::Motion(int button, int state, double x, double y)
{
  Mouse(button, state, x, y);
}
}
