#version 430 core

struct Light
{
  bool use;
  int type; // 0: directinoal, 1: point

  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  vec3 attenuation;
};

struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float shininess;
};

const int NUM_LIGHTS = 8;

uniform vec3 eye_position;
uniform Light lights[NUM_LIGHTS];

bool has_diffuse_texture;
uniform sampler2D diffuse_texture;

in vec2 frag_tex_coord;
in vec3 surface_position;
in vec3 surface_normal;
in vec3 surface_color;

out vec4 out_color;

vec3 colorDirectionalLight(Light light, Material material, vec3 N, vec3 V)
{
  vec3 L = normalize(light.position);
  vec3 R = - L + 2.0 * dot(L, N) * N;

  float NdotL = dot(N, L);
  float VdotR = dot(V, R);

  float diffuse_strength = clamp(NdotL, 0.f, 1.f);
  float specular_strength = pow( clamp(VdotR, 0.f, 1.f), material.shininess );

  vec3 ambient = light.ambient * material.ambient;
  vec3 diffuse = light.diffuse * diffuse * diffuse_strength;
  vec3 specular = light.specular * material.specular * specular_strength;

  return ambient + diffuse + specular;
}

vec3 colorPointLight(Light light, Material material, vec3 N, vec3 V)
{
  vec3 L = normalize(light.position - surface_position);
  vec3 R = - L + 2.0 * dot(L, N) * N;

  float NdotL = dot(N, L);
  float VdotR = dot(V, R);

  float diffuse_strength = clamp(NdotL, 0.f, 1.f);
  float specular_strength = pow( clamp(VdotR, 0.f, 1.f), material.shininess );

  // attenuation
  float distance = length(light.position - surface_position);
  float attenuation = 1.0 / (light.attenuation[0] + light.attenuation[1] * distance + light.attenuation[2] * (distance * distance));

  vec3 ambient = light.ambient * material.ambient;
  vec3 diffuse = light.diffuse * material.diffuse * diffuse_strength;
  vec3 specular = light.specular * material.specular * specular_strength;

  return (ambient + diffuse + specular) * attenuation;
}

vec3 fog(vec3 color)
{
  const float near = 0.7f;
  const float far = 0.9f;
  const vec3 background_color = vec3(0.8f, 0.8f, 0.8f);

  float depth = gl_FragCoord.z;

  if (depth > far)
    return background_color;
  else if (depth > near)
  {
    float t = (depth - near) / (far - near);
    return (1.f - t) * color + t * background_color;
  }
  return color;
}

void main()
{
  Material point_material;

  if (has_diffuse_texture)
  {
    point_material.diffuse = texture(material_texture, frag_tex_coord).rgb;
    point_material.ambient = texture(material_texture, frag_tex_coord).rgb;
  }
  else
  {
    point_material.diffuse = vec3(1.f, 0.f, 0.f);
    point_material.ambient = vec3(1.f, 0.f, 0.f);
  }

  vec3 N = normalize(surface_normal);
  vec3 V = normalize(eye_position - surface_position);

  vec3 total_color = vec3(0.f, 0.f, 0.f);

  for (int i=0; i<NUM_LIGHTS; i++)
  {
    if (lights[i].use)
    {
      if (lights[i].type == 0)
        total_color += colorDirectionalLight(lights[i], point_material, N, V);
      else if (lights[i].type == 1)
        total_color += colorPointLight(lights[i], point_material, N, V);
    }
  }

  out_color = vec4(fog(total_color), 1.f);
}
