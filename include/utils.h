#pragma once
#include <cstdint>
#include <string>

uint32_t ParseTime(const std::string &time);
bool IsValidName(const std::string &name);
std::string GetTimeString(uint32_t time);