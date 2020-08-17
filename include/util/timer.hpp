//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#if !defined(TIMER_HPP)
#define TIMER_HPP

#include <chrono>
#include <iostream>

class Timer {
private:
  typedef std::chrono::time_point<std::chrono::system_clock> time_t;

public:
  Timer() : startTime(), stopTime() {}

  time_t start() { return (startTime = std::chrono::system_clock::now()); }
  time_t stop() { return (stopTime = std::chrono::system_clock::now()); }
  // double elapsed() const { return std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count(); }
  // double elapsed() const { return (stopTime - startTime).count(); }
  double elapsed() const { return std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count(); }

friend std::ostream& operator<<(std::ostream& os, Timer& t);

private:
  time_t startTime, stopTime;
};

class empty_timer {
public:
  empty_timer(const std::string& msg = "") {}
  ~empty_timer() {}
};

class life_timer : public empty_timer {
private:
  typedef std::chrono::time_point<std::chrono::system_clock> time_t;

public:
  life_timer(const std::string& msg = "") : msg_(msg), startTime(), stopTime() { start(); }
  ~life_timer() {
    stop();
    if (msg_ != "") {
      std::cout << "# [ " + msg_ + " ]: ";
    }
    std::cout << elapsed() / 1.e3 << " ms" << std::endl;
  }

  time_t start() { return (startTime = std::chrono::system_clock::now()); }
  time_t stop() { return (stopTime = std::chrono::system_clock::now()); }
  // double elapsed() const { return std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime).count(); }
  // double elapsed() const { return (stopTime - startTime).count(); }
  double elapsed() const { return std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count(); }

private:
  std::string msg_;
  time_t      startTime, stopTime;
};

#define DEF_TIMER(label) Timer t_##label

#define START_TIMER(label) \
  do {                     \
    t_##label.start();     \
  } while (0)

#define STOP_TIMER(label)                                                                                   \
  do {                                                                                                      \
    t_##label.stop();                                                                                       \
    std::cout << "# elapsed time [" << #label << "]: " << t_##label.elapsed() / 1.e3 << " ms" << std::endl; \
  } while (0)

#define STOP_TIMER_QUIETLY(label) \
  {                               \
    [&](void) -> double {         \
      t_##label.stop();           \
      return t_##label.elapsed(); \
    }()                           \
  }


 std::ostream& operator<<(std::ostream& os, Timer& t) {
  os << " elapsed: " << t.elapsed();

  return os;
}

#endif    // TIMER_HPP
