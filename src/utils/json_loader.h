#ifndef SIMPLAN_UTILS_JSON_LOADER_H_
#define SIMPLAN_UTILS_JSON_LOADER_H_

#include <string>
#include <memory>

#include "utils/json.h"

namespace simplan
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

#endif // SIMPLAN_UTILS_JSON_LOADER_H_
