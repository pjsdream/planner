#ifndef SIMPLAN_RENDERING_OBJECT_ARRAY_OBJECT_H_
#define SIMPLAN_RENDERING_OBJECT_ARRAY_OBJECT_H_

#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rendering/object/buffer_object.h"

namespace simplan
{
class ArrayObject
{
public:
  ArrayObject();
  ~ArrayObject();

  void SetMode(GLenum mode)
  {
    mode_ = mode;
  }

  void SetNumElements(int num_elements)
  {
    num_elements_ = num_elements;
  }

  void SetAttribute(int index, int size, const std::shared_ptr<BufferObject>& vbo);
  void SetElementArrayBuffer(const std::shared_ptr<BufferObject>& vbo);
  void Draw();

private:
  GLuint id_;
  GLenum mode_;

  bool has_element_array_buffer_ = false;
  unsigned int num_elements_ = 0;
};
}

#endif // SIMPLAN_RENDERING_OBJECT_ARRAY_OBJECT_H_
