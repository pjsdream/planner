#ifndef SIMPLAN_VISION_KINECT_H_
#define SIMPLAN_VISION_KINECT_H_

#include <vector>

#include <Kinect.h>

namespace simplan
{
class Kinect
{
private:
  static const int COLOR_WIDTH_ = 1920;
  static const int COLOR_HEIGHT_ = 1080;

  static const int DEPTH_WIDTH_ = 512;
  static const int DEPTH_HEIGHT_ = 424;

public:
  Kinect();
  ~Kinect();

  bool InitializeSensor();

  void Update();

  std::vector<float> GetPointCloudPositionBuffer();
  std::vector<float> GetPointCloudColorBuffer();

private:
  IKinectSensor* sensor_;
  IMultiSourceFrameReader* frame_reader_;
  ICoordinateMapper* coordinate_mapper_;

  unsigned char* color_rgbx_buffer_ = nullptr;
  std::vector<float> depth_to_xyz_;
  std::vector<float> depth_to_rgb_;

  bool GetColorData(IMultiSourceFrame* frame);
  bool GetDepthData(IMultiSourceFrame* frame);
};
}

#endif // SIMPLAN_VISION_KINECT_H_
