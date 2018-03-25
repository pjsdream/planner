#ifndef SIMPLAN_RENDERING_OBJECT_BUFFER_OBJECT_H_
#define SIMPLAN_RENDERING_OBJECT_BUFFER_OBJECT_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace simplan
{
class BufferObject
{
public:
  BufferObject();
  ~BufferObject();

  auto GetId() const
  {
    return id_;
  }

  auto NumElements() const
  {
    return num_elements_;
  }

  void SetArrayBuffer(int size, void* data);
  void SetElementArrayBuffer(int size, int* data);

private:
  GLuint id_;
  GLuint num_elements_ = 0;
};
}

#endif // SIMPLAN_RENDERING_OBJECT_BUFFER_OBJECT_H_
