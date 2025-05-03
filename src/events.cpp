#include "events.h"
#include "utils.h"
#include <stdexcept>
#include <string>
#include <fstream>
#include <iostream>
Event::Event(int32_t id, uint32_t time, std::string client_name,
             uint32_t table_id)
    : id(id), time(time), client_name(std::move(client_name)),
      table_id(table_id) {}
Event::Event(int32_t id, uint32_t time, std::string client_name)
    : id(id), time(time), client_name(std::move(client_name)) {}

Event ValidateEvent(int32_t id, uint32_t time, std::string body,
                    uint32_t tables_cnt) {
  if (id < 1 || id > 4) {
    throw std::runtime_error("Incorrect ID in event");
  }
  if (id == 2) {
    auto pos_space = body.find(' ');
    if (pos_space == std::string::npos) {
      throw std::runtime_error("No space in event with ID = 2");
    }
    auto client_name = body.substr(0, pos_space);
    auto table_id_str = body.substr(pos_space + 1);
    if (!IsValidName(client_name)) {
      throw std::runtime_error("Incorrect name format");
    }
    uint32_t table_id = stoul(table_id_str);
    if (table_id > tables_cnt) {
      throw std::runtime_error("Incorrect table number");
    }
    return Event(id, time, std::move(client_name), table_id);
  } else {
    if (!IsValidName(body)) {
      throw std::runtime_error("Incorrect name format");
    }
    return Event(id, time, std::move(body));
  }
}

std::vector<Event> ReadEvents(std::ifstream &in, uint32_t tables_cnt) {
  std::vector<Event> events;
  std::string event;
  uint32_t prev_time_minutes = 0;
  while (std::getline(in, event)) {
    auto pos_first_space = event.find(' ');
    if (pos_first_space == std::string::npos) {
      throw std::runtime_error("Incorrect format in event (no spaces)");
    }
    auto pos_second_space = event.find(' ', pos_first_space + 1);
    if (pos_second_space == std::string::npos) {
      throw std::runtime_error(
          "Incorrect format in event (less than two spaces)");
    }
    std::string time, body, id_str;
    int32_t id;
    uint32_t time_minutes;
    time = event.substr(0, pos_first_space);
    id_str = event.substr(pos_first_space + 1,
                          pos_second_space - pos_first_space - 1);
    body = event.substr(pos_second_space + 1);
    time_minutes = ParseTime(time);

    if (prev_time_minutes > time_minutes) {
      throw std::runtime_error("Incorrect order of events");
    }
    prev_time_minutes = time_minutes;

    id = stol(id_str);

    events.emplace_back(
        ValidateEvent(id, time_minutes, std::move(body), tables_cnt));
  }
  return events;
}
