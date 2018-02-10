#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 frag_tex_coord;

void main()
{
  frag_tex_coord = tex_coord;
  gl_Position = projection * view * model * vec4(position, 1.0);
}