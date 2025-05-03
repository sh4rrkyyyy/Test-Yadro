#include "utils.h"
#include <stdexcept>
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