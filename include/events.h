#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
struct Event {
  Event(int32_t id, uint32_t time, std::string client_name, uint32_t table_id);
  Event(int32_t id, uint32_t time, std::string client_name);
  int32_t id;
  uint32_t time;
  std::string client_name;
  std::optional<uint32_t> table_id = std::nullopt;
};

Event ValidateEvent(int32_t id, uint32_t time, std::string body,
                    uint32_t tables_cnt);

std::vector<Event> ReadEvents(std::ifstream &in, uint32_t tables_cnt);
