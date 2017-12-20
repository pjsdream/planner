#include "renderer/window/renderer_context.h"
#include "renderer/window/renderer_window.h"

int main(int argc, char** argv)
{
  setbuf(stdout, NULL);

  renderer::RendererContext::Init(argc, argv);

  renderer::RendererWindow window;

  renderer::RendererContext::PrintVersion();
  renderer::RendererContext::Start();
  renderer::RendererContext::Finish();

  return 0;
}
