#ifndef PLANNER_SHAPE_SHAPE_H_
#define PLANNER_SHAPE_SHAPE_H_

namespace shape
{
class Shape
{
public:
  virtual ~Shape() = default;

  virtual bool IsCube() const
  {
    return false;
  }

private:
};
}

#endif // PLANNER_SHAPE_SHAPE_H_
