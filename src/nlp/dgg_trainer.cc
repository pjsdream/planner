#include "nlp/dgg_trainer.h"

#include "utils/json_loader.h"

namespace simplan
{
DggTrainer::DggTrainer()
{
}

DggTrainer::~DggTrainer()
{
}

void DggTrainer::AddTrainingData(const std::string& dataset_directory, const std::string& dataset_name)
{
  JsonLoader json_loader;
  auto dataset_json = json_loader.loadJson(dataset_directory + std::string("\\") + dataset_name + std::string("\\data.json"));

  for (auto data_json : dataset_json.toArray())
  {
    auto command_json = data_json["command"];
    auto nodes_json = data_json["nodes"];

    int num_nodes = nodes_json.size();

    // Generate a positive sample
    Data data;
    data.command = command_json.toString();

    for (auto node_json : nodes_json.toArray())
    {
      auto phrase_json = node_json["phrase"];
      auto pos_json = node_json["POS"];
      auto grounding_json = node_json["grounding"];
      auto grounding_params_json = node_json["grounding_parameters"];
      auto children_indices_json = node_json["children_indices"];

      Phrase phrase;

      phrase.SetPhrase(phrase_json.toString());
      phrase.SetPosFromString(pos_json.toString());

      data.lambda.push_back(phrase);

      Grounding grounding;

      std::vector<std::string> grounding_params;
      for (auto grounding_param_json : grounding_params_json.toArray())
        grounding_params.push_back(grounding_param_json.toString());

      grounding.SetGroundingFromString(grounding_json.toString(), grounding_params);

      data.gamma.push_back(grounding);
      data.phi.push_back(1);

      for (auto children_indices_node_json : children_indices_json.toArray())
      {
        std::vector<int> children_indices;

        for (auto index : children_indices_node_json.toArray())
          children_indices.push_back(index.toInt());

        data.children_indices.push_back(children_indices);
      }
    }

    dataset_.push_back(data);

    // Generate negative samples, with some phi = 0 (correspondences) and different gamma (groundings).
    double weight = -1. / static_cast<double>((1 << num_nodes) - 1)
    for (int bitmask = 0; bitmask < (1 << num_nodes) - 1; bitmask++)
    {
      Data negative_data = data;

      negative_data.phi[i]
    }
  }
}

void DggTrainer::Train()
{
}
}
