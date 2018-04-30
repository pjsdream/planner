#include "rendering/object/buffer_object.h"

namespace simplan
{
BufferObject::BufferObject()
{
  glGenBuffers(1, &id_);
}

BufferObject::~BufferObject()
{
  glDeleteBuffers(1, &id_);
}

void BufferObject::SetArrayBuffer(int size, void* data)
{
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferObject::AllocateDynamicArrayBuffer(int size)
{
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferObject::UpdateArrayBuffer(int size, void* data)
{
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferObject::SetElementArrayBuffer(int size, int* data)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
}
