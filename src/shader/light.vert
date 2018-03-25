#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat3 model_inverse_transpose;

out vec2 frag_tex_coord;
out vec3 surface_position;
out vec3 surface_normal;
out vec3 surface_color;

void main()
{
  vec4 model_position = model * vec4(position, 1.0);
  surface_position = vec3(model_position);
  surface_normal = model_inverse_transpose * normal;

  frag_tex_coord = tex_coord;

  gl_Position = projection * view * model_position;
}
