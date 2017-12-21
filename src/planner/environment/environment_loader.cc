#include "planner/environment/environment_loader.h"

#include <Eigen/Dense>

#include "planner/environment/object.h"
#include "json/json_loader.h"
#include "shape/cube.h"

namespace planner
{
std::shared_ptr<Environment> EnvironmentLoader::LoadEnvironmentFromFile(const std::string& filename)
{
  JsonLoader json_loader;
  Json json{json_loader.loadJson(filename)};

  auto environment = std::make_shared<Environment>();

  for (auto json_object : json.toArray())
  {
    std::shared_ptr<Object> object;
    auto name = json_object["name"].toString();
    auto type = json_object["type"].toString();
    auto geometry = json_object["geometry"];

    object = std::make_shared<Object>();

    std::shared_ptr<shape::Shape> shape;
    auto shape_type = geometry["shape"].toString();
    Eigen::Vector4f rgba{static_cast<float>(geometry["rgba"][0].toDouble()),
                         static_cast<float>(geometry["rgba"][1].toDouble()),
                         static_cast<float>(geometry["rgba"][2].toDouble()),
                         static_cast<float>(geometry["rgba"][3].toDouble())};
    Eigen::Vector3d xyz{geometry["xyz"][0].toDouble(), geometry["xyz"][1].toDouble(), geometry["xyz"][2].toDouble()};
    Eigen::Vector3d rpy{geometry["rpy"][0].toDouble(), geometry["rpy"][1].toDouble(), geometry["rpy"][2].toDouble()};

    Eigen::Affine3d transform = Eigen::Affine3d::Identity();
    transform.translate(xyz);
    transform.rotate(Eigen::AngleAxisd(rpy(0), Eigen::Vector3d(0, 0, 1)));
    transform.rotate(Eigen::AngleAxisd(rpy(1), Eigen::Vector3d(0, 1, 0)));
    transform.rotate(Eigen::AngleAxisd(rpy(2), Eigen::Vector3d(1, 0, 0)));

    if (shape_type == "cube")
    {
      Eigen::Vector3d
          size{geometry["size"][0].toDouble(), geometry["size"][1].toDouble(), geometry["size"][2].toDouble()};
      auto cube = std::make_shared<shape::Cube>(size);
      shape = cube;
    }

    if (shape != nullptr)
    {
      object->SetName(name);
      object->SetColor(rgba);
      object->SetShape(shape);
      object->SetTransform(transform);
      environment->AddObject(object);
    }
  }

  return environment;
}
}
