#include "nlp/grounding.h"

namespace simplan
{
Grounding::Grounding()
{
}

Grounding::~Grounding()
{
}

void Grounding::SetGroundingFromString(std::string grounding, std::vector<std::string> parameters)
{
  parameters_ = parameters;

  if (grounding == "negation")
    type_ = GroundingType::NEGATION;
  else if (grounding == "move")
    type_ = GroundingType::TASK_MOVE;
  else if (grounding == "object")
    type_ = GroundingType::OBJECT;
  else if (grounding == "on")
    type_ = GroundingType::SPATIAL_RELATION_ON;
  else if (grounding == "near")
    type_ = GroundingType::SPATIAL_RELATION_NEAR;
  else
    type_ = GroundingType::NO_MEANING;
}
}
