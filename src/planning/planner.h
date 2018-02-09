#ifndef SIMPLAN_PLANNING_PLANNER_H_
#define SIMPLAN_PLANNING_PLANNER_H_

#include <string>

namespace simplan
{
class Planner
{
public:
  Planner() = delete;
  Planner(const std::string& config_filename);
  ~Planner();

private:
};
}

#endif // SIMPLAN_PLANNING_PLANNER_H_
