#include "json/json.h"

namespace planner
{
Json::Json()
    : type_(Type::Undefined)
{
}

Json::Json(long long v)
    : type_(Type::Int), int_value_(v)
{
}

Json::Json(int v)
    : type_(Type::Int), int_value_(v)
{
}

Json::Json(bool v)
    : type_(Type::Bool), bool_value_(v)
{
}

Json::Json(double v)
    : type_(Type::Double), double_value_(v)
{
}

Json::Json(const std::string& v)
    : type_(Type::String), string_value_(v)
{
}

Json::Json(std::string&& v)
    : type_(Type::String), string_value_(std::move(v))
{
}

long long Json::toInt()
{
  switch (type_)
  {
    case Type::Int:
      return int_value_;
    case Type::Bool:
      return bool_value_;
    case Type::Double:
      return double_value_;
    default:
      return 0L;
  }
}

bool Json::toBool()
{
  switch (type_)
  {
    case Type::Bool:
      return bool_value_;
    case Type::Int:
      return int_value_ != 0;
    case Type::Double:
      return double_value_ != 0.;
    default:
      return false;
  }
}

double Json::toDouble()
{
  switch (type_)
  {
    case Type::Double:
      return double_value_;
    case Type::Int:
      return int_value_;
    case Type::Bool:
      return bool_value_;
    default:
      return 0.;
  }
}

std::string Json::toString()
{
  switch (type_)
  {
    case Type::String:
      return string_value_;
    default:
      return "";
  }
}

const std::vector<Json>& Json::toArray() const
{
  return array_;
}

std::vector<Json>& Json::toArray()
{
  return array_;
}

const std::unordered_map<std::string, std::shared_ptr<Json>>& Json::toObject() const
{
  return object_;
}

void Json::set(bool v)
{
  clear();
  type_ = Type::Bool;
  bool_value_ = v;
}

void Json::set(int v)
{
  clear();
  type_ = Type::Int;
  int_value_ = v;
}

void Json::set(long long v)
{
  clear();
  type_ = Type::Int;
  int_value_ = v;
}

void Json::set(double v)
{
  clear();
  type_ = Type::Double;
  double_value_ = v;
}

void Json::set(const std::string& v)
{
  clear();
  type_ = Type::String;
  string_value_ = v;
}

Json& Json::operator[](int index)
{
  type_ = Type::Array;
  object_.clear();

  return array_[index];
}

Json& Json::operator[](const std::string& key)
{
  type_ = Type::Object;
  array_.clear();

  if (object_.find(key) == object_.cend())
    object_[key] = std::make_shared<Json>();
  return *object_[key];
}

Json& Json::operator[](std::string&& key)
{
  type_ = Type::Object;
  array_.clear();

  if (object_.find(key) == object_.cend())
    object_[key] = std::make_shared<Json>();
  return *object_[key];
}

Json& Json::operator=(int v)
{
  set(v);
  return *this;
}

Json& Json::operator=(bool v)
{
  set(v);
  return *this;
}

Json& Json::operator=(double v)
{
  set(v);
  return *this;
}

Json& Json::operator=(const char* v)
{
  set(std::string(v));
  return *this;
}

Json& Json::operator=(const std::string& v)
{
  set(v);
  return *this;
}

Json& Json::operator=(std::string&& v)
{
  set(std::move(v));
  return *this;
}

unsigned long Json::size()
{
  switch (type_)
  {
    case Type::Array:
      return array_.size();
    case Type::Object:
      return object_.size();
    default:
      return 0;
  }
}

void Json::add(const Json& value)
{
  if (type_ != Type::Array)
  {
    clear();
    type_ = Type::Array;
  }
  array_.push_back(value);
}

void Json::add(Json&& value)
{
  if (type_ != Type::Array)
  {
    clear();
    type_ = Type::Array;
  }
  array_.emplace_back(value);
}

bool Json::containsKey(const std::string& key)
{
  return object_.find(key) != object_.cend();
}

bool Json::containsKey(std::string&& key)
{
  return object_.find(std::move(key)) != object_.cend();
}

void Json::clear()
{
  array_.clear();
  object_.clear();
}

std::string Json::toPrettyString()
{
  return toPrettyString(0, false);
}

std::string Json::toPrettyString(int indent, bool indent_at_beginning)
{
  switch (type_)
  {
    case Type::Int:
      return (indent_at_beginning ? std::string(indent, ' ') : "") + std::to_string(int_value_);

    case Type::Bool:
      return (indent_at_beginning ? std::string(indent, ' ') : "") + (bool_value_ ? "true" : "false");

    case Type::Double:
      return (indent_at_beginning ? std::string(indent, ' ') : "") + std::to_string(double_value_);

    case Type::String:
      return (indent_at_beginning ? std::string(indent, ' ') : "") + "\"" + string_value_+ "\"";

    case Type::Array:
    {
      std::string result = indent_at_beginning ? std::string(indent, ' ') + "[\n" : "[\n";
      for (int i=0; i<array_.size(); i++)
      {
        result += array_[i].toPrettyString(indent + 1, true);
        if (i < array_.size() - 1)
          result += ",\n";
      }
      result += "\n" + std::string(indent, ' ') + "]";

      return result;
    }

    case Type::Object:
    {
      std::string result = indent_at_beginning ? std::string(indent, ' ') + "{\n" : "{\n";
      int cnt = 0;
      for (auto kv : object_)
      {
        result += std::string(indent+1, ' ') + "\"" + kv.first + "\": " + kv.second->toPrettyString(indent + 1, false);
        if (cnt < object_.size() - 1)
          result += ",\n";
        cnt++;
      }
      result += "\n" + std::string(indent, ' ') + "}";

      return result;
    }

    default:
      return "(undefined)";
  }
}
}
