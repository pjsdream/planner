#ifndef SIMPLAN_NLP_GROUNDING_H_
#define SIMPLAN_NLP_GROUNDING_H_

#include <string>
#include <vector>

namespace simplan
{
class Grounding
{
public:
  enum class GroundingType
  {
    UNDEFINED,
    NEGATION,
    TASK_MOVE,
    OBJECT,
    SPATIAL_RELATION_ON,
    SPATIAL_RELATION_NEAR,
    NO_MEANING,
  };

public:
  Grounding();
  ~Grounding();

  void SetGroundingFromString(std::string grounding, std::vector<std::string> parameters = std::vector<std::string>());

private:
  GroundingType type_ = GroundingType::UNDEFINED;
  std::vector<std::string> parameters_;
};
}

#endif // SIMPLAN_NLP_GROUNDING_H_
