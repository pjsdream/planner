#ifndef SIMPLAN_RESOURCE_TEXTURE_H_
#define SIMPLAN_RESOURCE_TEXTURE_H_

#include <vector>

namespace simplan
{
struct Texture
{
  unsigned int width;
  unsigned int height;
  std::vector<unsigned char> image;
};
}

#endif // SIMPLAN_RESOURCE_TEXTURE_H_
