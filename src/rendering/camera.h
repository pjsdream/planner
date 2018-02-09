#ifndef SIMPLAN_RENDERING_CAMERA_H_
#define SIMPLAN_RENDERING_CAMERA_H_

namespace simplan
{
class Camera
{
public:
  Camera();
  ~Camera();

  void MoveByPixel(int dx, int dy);

private:
};
}

#endif // SIMPLAN_RENDERING_CAMERA_H_
