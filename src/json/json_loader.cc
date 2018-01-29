#include "json/json_loader.h"

#include <iostream>
#include <stack>

namespace planner
{
enum class State
{
  MinusOrNone,
  ZeroOrDigit,
  Digit,
  DotOrNone,
  DigitAfterDot,
  ExponentOrNone,
  ExponentSignOrNone,
  DigitAfterExponent,
  End
};

Json JsonLoader::loadJson(const std::string& filename)
{
  json_string_.clear();

  std::FILE* fp = fopen(filename.c_str(), "r");
  char buffer[1024];
  while (fgets(buffer, sizeof buffer, fp) != NULL)
    json_string_ += buffer;
  std::fclose(fp);

  int x = 0;
  return parseJsonFromString(x);
}

Json JsonLoader::parseJsonFromString(int& x)
{
  moveIndexToNonWhitespace(x);

  switch (json_string_[x])
  {
    // string
    case '\"':
    {
      return parseJsonStringFromString(x);
    }

      // bool(true)
    case 't':
    {
      x += 4;
      return Json(true);
    }

      // bool(false)
    case 'f':
    {
      x += 5;
      return Json(false);
    }

      // array
    case '[':
    {
      return parseJsonArrayFromString(x);
    }

      // object
    case '{':
    {
      return parseJsonObjectFromString(x);
    }

      // number
    default:
    {
      State state = State::MinusOrNone;
      int sign = 1;
      long long int_value = 0;
      double double_value = 0;
      double double_digit = 0.1;
      bool is_int_value = true;
      int exponent_sign = 1;
      int exponent_number = 0;
      while (x < json_string_.size() && state != State::End)
      {
        switch (state)
        {
          case State::MinusOrNone:
          {
            if (json_string_[x] == '-')
            {
              sign = -1;
              x++;
            }
            state = State::ZeroOrDigit;
          }
            break;

          case State::ZeroOrDigit:
          {
            if (json_string_[x] == '0')
            {
              state = State::DotOrNone;
              x++;
            }
            else if ('1' <= json_string_[x] && json_string_[x] <= '9')
            {
              state = State::Digit;
              int_value = int_value * 10 + json_string_[x] - '0';
              x++;
            }
            else
              throw "JsonLoader failed: expected [0-9].";
          }
            break;

          case State::Digit:
          {
            if (isDigit(json_string_[x]))
            {
              int_value = int_value * 10 + json_string_[x] - '0';
              x++;
            }
            else
              state = State::DotOrNone;
          }
            break;

          case State::DotOrNone:
          {
            if (json_string_[x] == '.')
            {
              state = State::DigitAfterDot;
              is_int_value = false;
              double_value = int_value;
              x++;
            }
            else
              state = State::ExponentOrNone;
          }
            break;

          case State::DigitAfterDot:
          {
            if (isDigit(json_string_[x]))
            {
              double_value += (json_string_[x] - '0') * double_digit;
              double_digit *= 0.1;
              x++;
            }
            else
              state = State::ExponentOrNone;
          }
            break;

          case State::ExponentOrNone:
          {
            if (json_string_[x] == 'e' || json_string_[x] == 'E')
            {
              if (is_int_value)
              {
                is_int_value = false;
                double_value = int_value;
              }
              state = State::ExponentSignOrNone;
              x++;
            }
            else
              state = State::End;
          }
            break;

          case State::ExponentSignOrNone:
          {
            if (json_string_[x] == '+')
              x++;
            else if (json_string_[x] == '-')
            {
              exponent_sign = -1;
              x++;
            }
            state = State::DigitAfterExponent;
          }
            break;

          case State::DigitAfterExponent:
          {
            if (isDigit(json_string_[x]))
            {
              exponent_number = exponent_number * 10 + json_string_[x] - '0';
              x++;
            }
            else
              state = State::End;
          }
          break;

          default:
            break;
        }
      }

      if (is_int_value)
        return Json(sign * int_value);
      else
      {
        if (exponent_sign == 1)
        {
          for (int i=0; i<exponent_number; i++)
            double_value *= 10.;
        }
        else
        {
          for (int i=0; i<exponent_number; i++)
            double_value /= 10.;
        }
        return Json(sign * double_value);
      }
    }
  }
}

Json JsonLoader::parseJsonStringFromString(int& x)
{
  moveIndexToNonWhitespace(x);
  if (json_string_[x] != '\"')
    throw "JsonLoader failed: expected \'\"\'";
  x++;

  std::string string_value;
  while (json_string_[x] != '\"')
  {
    if (json_string_[x] == '\\')
    {
      // TODO
    }
    else
      string_value += json_string_[x++];
  }

  x++;
  return Json(string_value);
}

Json JsonLoader::parseJsonArrayFromString(int& x)
{
  moveIndexToNonWhitespace(x);
  if (json_string_[x] != '[')
    throw "JsonLoader failed: expected \'[\'";
  x++;

  Json json{};

  while (true)
  {
    json.add(std::move(parseJsonFromString(x)));

    moveIndexToNonWhitespace(x);
    if (json_string_[x] == ']')
    {
      x++;
      return json;
    }

    if (json_string_[x] == ',')
    {
      x++;
    }
    else
      throw "JsonLoader failed: expected \',\' or \'}\'";
  }

  return json;
}

Json JsonLoader::parseJsonObjectFromString(int& x)
{
  moveIndexToNonWhitespace(x);
  if (json_string_[x] != '{')
    throw "JsonLoader failed: expected \'{\'";
  x++;

  Json json{};

  while (true)
  {
    auto name = parseJsonStringFromString(x).toString();

    moveIndexToNonWhitespace(x);
    if (json_string_[x] != ':')
      throw "JsonLoader failed: expected \':\'";
    x++;

    auto element = parseJsonFromString(x);
    json[name] = element;

    moveIndexToNonWhitespace(x);
    if (json_string_[x] == '}')
    {
      x++;
      return json;
    }
    if (json_string_[x] == ',')
    {
      x++;
    }
    else
      throw "JsonLoader failed: expected \',\' or \'}\'";
  }

  return json;
}

void JsonLoader::moveIndexToNonWhitespace(int& x)
{
  while (x < json_string_.size() && isWhitespace(json_string_[x]))
    x++;
}

bool JsonLoader::isWhitespace(char x)
{
  return x == '\n' || x == '\r' || x == ' ' || x == '\t';
}

bool JsonLoader::isDigit(char x)
{
  return '0' <= x && x <= '9';
}
}
