#include "utils.h"
#include <stdexcept>
#include <sys/types.h>
#include "constants.h"
uint32_t ParseTime(const std::string &time) {
  if (time.size() != 5 || time[2] != ':') {
    throw std::runtime_error("Incorrect time format");
  }
  uint32_t hours = std::stoul(time.substr(0, 2));
  if (hours >= 24) {
    throw std::runtime_error("Incorrect time format (hours)");
  }
  uint32_t minutes = std::stoul(time.substr(3, 2));
  if (minutes >= 60) {
    throw std::runtime_error("Incorrect time format (minutes)");
  }
  return hours * 60 + minutes;
}

bool IsValidName(const std::string &name) {
  if (name.find_first_not_of(constants::NameSymbols) == std::string::npos) {
    return true;
  }
  return false;
}


std::string GetTimeString(uint32_t time) {
  auto hours = std::to_string(time / 60);
  auto minutes = std::to_string(time % 60);
  return std::string(2 - hours.size(), '0') + hours + ":" +
         std::string(2 - minutes.size(), '0') + minutes;
}