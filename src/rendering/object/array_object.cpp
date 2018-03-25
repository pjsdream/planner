#include "rendering/object/array_object.h"

namespace simplan
{
ArrayObject::ArrayObject()
{
  glGenVertexArrays(1, &id_);
}

ArrayObject::~ArrayObject()
{
  glDeleteVertexArrays(1, &id_);
}

void ArrayObject::SetAttribute(int index, int size, const std::shared_ptr<BufferObject>& vbo)
{
  glBindVertexArray(id_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo->GetId());
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(index);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void ArrayObject::SetElementArrayBuffer(const std::shared_ptr<BufferObject>& vbo)
{
  glBindVertexArray(id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->GetId());
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  has_element_array_buffer_ = true;
}

void ArrayObject::Draw()
{
  glBindVertexArray(id_);

  if (has_element_array_buffer_)
  {
    glDrawElements(mode_, num_elements_, GL_UNSIGNED_INT, (void*)0);
  }
  else
  {
    glDrawArrays(mode_, 0, num_elements_);
  }

  glBindVertexArray(0);
}
}
