#ifndef PLANNER_JSON_JSON_LOADER_H_
#define PLANNER_JSON_JSON_LOADER_H_

#include "json/json.h"

#include <string>
#include <memory>

namespace planner
{
class JsonLoader
{
public:
  Json loadJson(const std::string& filename);

private:
  Json parseJsonFromString(int& x);
  Json parseJsonStringFromString(int& x);
  Json parseJsonArrayFromString(int& x);
  Json parseJsonObjectFromString(int& x);
  void moveIndexToNonWhitespace(int& x);

  static bool isWhitespace(char x);
  static bool isDigit(char x);

  std::string json_string_;
};
}

#endif // PLANNER_JSON_JSON_LOADER_H_
