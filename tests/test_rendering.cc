#include "rendering/rendering_window.h"

#include <memory>

int main()
{
  auto window = std::make_shared<simplan::RenderingWindow>();
  window->Run();

  return 0;
}
