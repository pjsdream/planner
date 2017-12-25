#ifndef PLANNER_UTILS_FUTURE_WRAPPER_H_
#define PLANNER_UTILS_FUTURE_WRAPPER_H_

#include <future>

namespace planner
{
//
// Windows
//
#if defined(_WIN32) || defined(_WIN64)
enum class launch
{
  async = 1,
  deferred = 2
};

template<typename T>
class future
{
public:
  future()
      : valid_(false)
  {
  }

  T get()
  {
    return value_;
  }

  bool valid()
  {
    return valid_;
  }

  template<class Rep, class Period>
  std::future_status wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const
  {
  }

  void _set_value(T v)
  {
    value_ = v;
    valid_ = true;
  }

private:
  T value_;
  bool valid_;
};

template<>
class future<void>
{
public:
  future()
      : valid_(false)
  {
  }

  void get()
  {
  }

  bool valid()
  {
    return valid_;
  }

  template<class Rep, class Period>
  std::future_status wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const
  {
  }

  void _set_value()
  {
    valid_ = true;
  }

private:
  bool valid_;
};

template<class Function, class... Args, typename = typename std::enable_if_t<!std::is_same<std::result_of_t<std::decay_t<
    Function>(std::decay_t<Args>...)>, void>::value>>
future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>>
async(launch policy, Function&& f, Args&& ... args)
{
  using return_type = typename std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>;
  future<return_type> result;

  result._set_value(f(args...));

  return result;
}

template<class Function, class... Args, typename = typename std::enable_if_t<std::is_same<std::result_of_t<std::decay_t<
    Function>(std::decay_t<Args>...)>, void>::value>>
future<void> async(launch policy, Function&& f, Args&& ... args)
{
  future<void> result;

  f(args...);
  result._set_value();

  return result;
}

//
// Linux
//
#else
template<typename T>
using future = std::future<T>;

using auto async = std::async;

#endif
}

#endif // PLANNER_UTILS_FUTURE_WRAPPER_H_
