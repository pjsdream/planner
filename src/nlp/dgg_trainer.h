#ifndef SIMPLAN_NLP_DGG_TRAINER_H_
#define SIMPLAN_NLP_DGG_TRAINER_H_

#include <string>
#include <vector>
#include <memory>

#include "nlp/phrase.h"
#include "nlp/grounding.h"
#include "nlp/dgg_cost_parameters.h"
#include "nlp/environment.h"

namespace simplan
{
class DggTrainer
{
private:
  struct Data
  {
    // The number of positive samples are much less than negative samples.
    // To compensate it, set lower weights on negative samples.
    double weight = 1.;

    std::string command;

    // Phrase is a sequence of words, with a part-of-speech tag.
    std::vector<Phrase> lambda;

    // Groundings
    std::vector<Grounding> gamma;

    // Correspondance variables
    std::vector<unsigned char> phi;

    // Node index starts with 0.
    // Indices of child nodes of node i are children_indices_[i].
    std::vector<std::vector<int>> children_indices;

    // Cost parameters for robot motion planner.
    // As a DGG module, cost parameters are just a set of real values.
    DggCostParameters parameters;

    // Environment
    Environment environment;
  };

public:
  DggTrainer();
  ~DggTrainer();

  // Example: directory = "C:\\planner\\data", dataset_name = "target_recognition"
  void AddTrainingData(const std::string& directory, const std::string& dataset_name);

  void Train();

private:
  std::vector<Data> dataset_;
};
}

#endif // SIMPLAN_NLP_DGG_TRAINER_H_
