#ifndef RENDERER_WIDGET_TEXT_WIDGET_H_
#define RENDERER_WIDGET_TEXT_WIDGET_H_

#include "renderer/widget/widget.h"

#include <memory>
#include <vector>

namespace renderer
{
class TextWidget : public Widget
{
public:
  TextWidget(const std::string& text);

private:
  std::string text_;
};
}

#endif // RENDERER_WIDGET_TEXT_WIDGET_H_
