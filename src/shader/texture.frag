#version 330 core

in vec2 frag_tex_coord;

uniform sampler2D material_texture;

out vec4 out_color;

void main()
{
  out_color = vec4(texture(material_texture, frag_tex_coord).rgb, 1.f);
}
