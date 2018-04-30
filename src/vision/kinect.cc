#include "vision/kinect.h"

#include <iostream>
#include <thread>

namespace simplan
{
Kinect::Kinect()
{
}

Kinect::~Kinect()
{
  if (color_rgbx_buffer_ != nullptr)
    delete color_rgbx_buffer_;
}

bool Kinect::InitializeSensor()
{
  HRESULT hr;

  hr = GetDefaultKinectSensor(&sensor_);
  if (FAILED(hr))
  {
    return hr;
  }

  if (sensor_)
  {
    // Coordinate mapper
    hr = sensor_->get_CoordinateMapper(&coordinate_mapper_);

    if (SUCCEEDED(hr))
      hr = sensor_->Open();

    if (SUCCEEDED(hr))
      hr = sensor_->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color, &frame_reader_);
  }

  if (!sensor_ || FAILED(hr))
  {
    std::cerr << "No ready Kinect found!\n";
    return false;
  }

  if (hr == S_OK)
  {
    color_rgbx_buffer_ = new unsigned char[COLOR_WIDTH_ * COLOR_HEIGHT_ * 4];
  }

  return hr == S_OK;
}

void Kinect::Update()
{
  if (!frame_reader_)
    return;

  IMultiSourceFrame* frame = NULL;
  if (SUCCEEDED(frame_reader_->AcquireLatestFrame(&frame)))
  {
    GetDepthData(frame);
    GetColorData(frame);
  }

  if (frame != NULL)
    frame->Release();
}

bool Kinect::GetColorData(IMultiSourceFrame* frame)
{
  HRESULT hr;
  IColorFrame* color_frame;
  IColorFrameReference* color_frame_reference = NULL;

  if (FAILED(frame->get_ColorFrameReference(&color_frame_reference)))
    return false;

  hr = color_frame_reference->AcquireFrame(&color_frame);
  if (FAILED(hr))
    return false;

  color_frame->CopyConvertedFrameDataToArray(COLOR_WIDTH_ * COLOR_HEIGHT_ * 4, reinterpret_cast<BYTE*>(color_rgbx_buffer_), ColorImageFormat_Bgra);

  if (color_frame_reference != NULL)
    color_frame_reference->Release();

  if (color_frame!= NULL)
    color_frame->Release();

  return true;
}

bool Kinect::GetDepthData(IMultiSourceFrame* frame)
{
  IDepthFrame* depth_frame;
  IDepthFrameReference* depth_frame_reference = NULL;

  if (FAILED(frame->get_DepthFrameReference(&depth_frame_reference)))
    return false;

  if (FAILED(depth_frame_reference->AcquireFrame(&depth_frame)))
    return false;

  unsigned int size;
  unsigned short* buffer;
  if (FAILED(depth_frame->AccessUnderlyingBuffer(&size, &buffer)))
    return false;

  depth_to_xyz_.resize(DEPTH_WIDTH_ * DEPTH_HEIGHT_ * 3, 0.f);
  coordinate_mapper_->MapDepthFrameToCameraSpace(DEPTH_WIDTH_ * DEPTH_HEIGHT_, buffer, DEPTH_WIDTH_ * DEPTH_HEIGHT_, reinterpret_cast<CameraSpacePoint*>(depth_to_xyz_.data()));

  depth_to_rgb_.resize(DEPTH_WIDTH_ * DEPTH_HEIGHT_ * 2, 0.f);
  coordinate_mapper_->MapDepthFrameToColorSpace(DEPTH_WIDTH_ * DEPTH_HEIGHT_, buffer, DEPTH_WIDTH_ * DEPTH_HEIGHT_, reinterpret_cast<ColorSpacePoint*>(depth_to_rgb_.data()));

  if (depth_frame_reference != NULL)
    depth_frame_reference->Release();

  if (depth_frame != NULL)
    depth_frame->Release();

  return true;
}

std::vector<float> Kinect::GetPointCloudPositionBuffer()
{
  std::vector<float> position;
  if (depth_to_xyz_.empty())
    return position;

  position.resize(DEPTH_WIDTH_ * DEPTH_HEIGHT_ * 3);
  for (int i = 0; i < DEPTH_WIDTH_ * DEPTH_HEIGHT_; i++)
  {
    position[3 * i + 0] = depth_to_xyz_[3 * i + 0];
    position[3 * i + 1] = depth_to_xyz_[3 * i + 1];
    position[3 * i + 2] = depth_to_xyz_[3 * i + 2];
  }

  return position;
}

std::vector<float> Kinect::GetPointCloudColorBuffer()
{
  std::vector<float> color;
  if (depth_to_rgb_.empty())
    return color;

  color.resize(DEPTH_WIDTH_ * DEPTH_HEIGHT_ * 3);

  int idx = 0;
  for (int i = 0; i < DEPTH_WIDTH_ * DEPTH_HEIGHT_; i++)
  {
    float x = depth_to_rgb_[i * 2];
    float y = depth_to_rgb_[i * 2 + 1];

    if (x < 0.f || y < 0.f || x >= COLOR_WIDTH_ || y >= COLOR_HEIGHT_)
    {
      color[idx++] = 0.;
      color[idx++] = 0.;
      color[idx++] = 0.;
    }
    else
    {
      int bidx = (int)x + COLOR_WIDTH_ * (int)y;
      color[idx++] = color_rgbx_buffer_[bidx * 4 + 0] / 255.f;
      color[idx++] = color_rgbx_buffer_[bidx * 4 + 1] / 255.f;
      color[idx++] = color_rgbx_buffer_[bidx * 4 + 2] / 255.f;
    }
  }

  return color;
}
}
