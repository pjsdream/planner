#include "resource/texture_manager.h"

#include <unordered_map>
#include <future>

#include <lodepng/lodepng.h>

namespace simplan
{
namespace
{
std::unordered_map<std::string, std::shared_ptr<Texture>> filename_to_texture;
std::unordered_map<std::string, std::future<Texture>> filename_to_future_texture;
}

std::shared_ptr<Texture> TextureManager::LoadFromFile(const std::string& filename)
{
  if (filename_to_texture.find(filename) != filename_to_texture.cend())
    return filename_to_texture[filename];

  if (filename_to_future_texture.find(filename) != filename_to_future_texture.cend())
  {
    auto& f = filename_to_future_texture[filename];
    if (f.valid() && f.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
    {
      auto texture = std::make_shared<Texture>(f.get());
      filename_to_texture[filename] = texture;
      filename_to_future_texture.erase(filename);
      return texture;
    }
  }

  filename_to_future_texture.insert(std::make_pair(filename, std::async(std::launch::async, [filename]()
  {
    Texture texture;
    lodepng::decode(texture.image, texture.width, texture.height, filename.c_str());
    return texture;
  })));

  return nullptr;
}
}
