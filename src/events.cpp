#include "events.h"
#include "constants.h"
#include "table.h"
#include "utils.h"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

Event::Event(int32_t id, uint32_t time, std::string client_name,
             uint32_t table_id)
    : client_name_(std::move(client_name)), id_(id), time_(time),
      table_id_(table_id) {}

Event::Event(int32_t id, uint32_t time, std::string client_name)
    : client_name_(std::move(client_name)), id_(id), time_(time) {}

uint32_t Event::GetTimeMinutes() const { return time_; }

int32_t Event::GetID() const { return id_; }

std::string Event::GetClientName() const { return client_name_; }

std::optional<uint32_t> Event::GetTableID() const { return table_id_; }

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
    return Event(id, time, std::move(client_name), table_id - 1);
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

void HandleEventsID1(std::unordered_map<ClientName, Client> &clients,
                     uint32_t start_time, uint32_t end_time,
                     const Event &event) {
  if (clients.find(event.GetClientName()) != clients.end()) {
    std::cout << GetTimeString(event.GetTimeMinutes()) << ' '
              << constants::ErrorEventID << ' ' << constants::ErrorAlreadyInClub
              << std::endl;
  } else if (event.GetTimeMinutes() < start_time ||
             event.GetTimeMinutes() >= end_time) {
    std::cout << GetTimeString(event.GetTimeMinutes()) << ' '
              << constants::ErrorEventID << ' ' << constants::ErrorNotOpen
              << std::endl;
  } else {
    clients.insert({event.GetClientName(), Client(event.GetClientName())});
  }
}

void HandleEventsID2(std::unordered_map<ClientName, Client> &clients,
                     std::vector<Table> &tables, const Event &event) {
  if (clients.find(event.GetClientName()) == clients.end()) {
    std::cout << GetTimeString(event.GetTimeMinutes()) << ' '
              << constants::ErrorEventID << ' ' << constants::ErrorNotInClub
              << std::endl;
    return;
  }
  auto table_id_opt = event.GetTableID();
  if (!table_id_opt) {
    throw std::runtime_error("No table for event with ID = 2");
  }
  auto table_id = *table_id_opt;
  if (tables[table_id].Occupied()) {
    std::cout << GetTimeString(event.GetTimeMinutes()) << ' '
              << constants::ErrorEventID << ' ' << constants::ErrorOccupiedTable
              << std::endl;
    return;
  }
  auto old_table_id = clients[event.GetClientName()].GetTableID();
  if (old_table_id) {
    tables[*old_table_id].UpdateTotalMinutesAndRevenue(event.GetTimeMinutes());
  }
  tables[table_id].SetStartMinutes(event.GetTimeMinutes());
  clients[event.GetClientName()].SetTableID(table_id);
}

void HandleEventsID3(std::vector<Table> &tables, std::queue<ClientName> &queue,
                     uint32_t tables_cnt, const Event &event) {
  if (std::find_if(tables.begin(), tables.end(), [](const Table &table) {
        return !table.Occupied();
      }) != tables.end()) {
    std::cout << GetTimeString(event.GetTimeMinutes()) << ' '
              << constants::ErrorEventID << ' '
              << constants::ErrorExistFreeTables << std::endl;
    return;
  }
  if (queue.size() > tables_cnt) {
    std::cout << GetTimeString(event.GetTimeMinutes()) << ' '
              << constants::LeaveEventID << ' ' << event.GetClientName()
              << std::endl;
    return;
  }
  queue.push(event.GetClientName());
}

void HandleEventsID4(std::unordered_map<ClientName, Client> &clients,
                     std::vector<Table> &tables, std::queue<ClientName> &queue,
                     const Event &event) {
  if (clients.find(event.GetClientName()) == clients.end()) {
    std::cout << GetTimeString(event.GetTimeMinutes()) << ' '
              << constants::ErrorEventID << ' ' << constants::ErrorNotInClub
              << std::endl;
    return;
  }
  auto table_id_opt = clients[event.GetClientName()].GetTableID();
  if (!table_id_opt) {
    clients.erase(event.GetClientName());
  } else {
    auto table_id = *table_id_opt;
    tables[table_id].UpdateTotalMinutesAndRevenue(event.GetTimeMinutes());
    clients.erase(event.GetClientName());
    while (!queue.empty()) {
      auto client_name = queue.front();
      queue.pop();
      if (clients.find(client_name) == clients.end()) {
        continue;
      }
      clients[client_name].SetTableID(table_id);
      tables[table_id].SetStartMinutes(event.GetTimeMinutes());
      std::cout << GetTimeString(event.GetTimeMinutes()) << ' '
                << constants::SeatEventID << ' ' << client_name << ' '
                << table_id + 1 << std::endl;
      break;
    }
  }
}

void HandleEndOfDayEvents(std::unordered_map<ClientName, Client> &clients,
                          std::vector<Table> &tables, uint32_t end_time) {
  std::vector<ClientName> client_names;
  for (const auto &[client_name, _] : clients) {
    client_names.push_back(client_name);
  }
  std::sort(client_names.begin(), client_names.end());
  for (const auto &client_name : client_names) {
    auto table_id_opt = clients[client_name].GetTableID();
    if (table_id_opt) {
      auto table_id = *table_id_opt;
      tables[table_id].UpdateTotalMinutesAndRevenue(end_time);
    }
    std::cout << GetTimeString(end_time) << ' ' << constants::LeaveEventID
              << ' ' << client_name << std::endl;
  }
  clients.clear();
}

void HandleEvents(std::vector<Event> &events,
                  std::unordered_map<ClientName, Client> &clients,
                  std::vector<Table> &tables, std::queue<ClientName> &queue,
                  uint32_t start_time, uint32_t end_time, uint32_t tables_cnt) {
  for (const auto &event : events) {
    auto event_id = event.GetID();
    std::cout << GetTimeString(event.GetTimeMinutes()) << ' ' << event_id << ' '
              << event.GetClientName();
    if (event_id == 2) {
      std::cout << ' ' << *event.GetTableID() + 1 << std::endl;
    } else {
      std::cout << std::endl;
    }
    if (event_id == 1) {
      HandleEventsID1(clients, start_time, end_time, event);
    } else if (event_id == 2) {
      HandleEventsID2(clients, tables, event);
    } else if (event_id == 3) {
      HandleEventsID3(tables, queue, tables_cnt, event);
    } else if (event_id == 4) {
      HandleEventsID4(clients, tables, queue, event);
    }
  }
  HandleEndOfDayEvents(clients, tables, end_time);
}
