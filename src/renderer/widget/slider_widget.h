#ifndef RENDERER_WIDGET_SLIDER_WIDGET_H_
#define RENDERER_WIDGET_SLIDER_WIDGET_H_

#include "renderer/widget/widget.h"

#include <memory>
#include <vector>

namespace renderer
{
class SliderWidget : public Widget
{
public:
  SliderWidget(double min_value, double max_value, double default_value);

  void Mouse(int button, int state, double x, double y) override;
  void Motion(int button, int state, double x, double y) override;

private:
  double min_value_;
  double max_value_;
  double value_;
};
}

#endif // RENDERER_WIDGET_SLIDER_WIDGET_H_
