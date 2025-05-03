#pragma once
#include "client.h"
#include "table.h"
#include <cstdint>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

class Event {
public:
  Event(int32_t id, uint32_t time, std::string client_name, uint32_t table_id);
  Event(int32_t id, uint32_t time, std::string client_name);
  uint32_t GetTimeMinutes() const;
  int32_t GetID() const;
  std::string GetClientName() const;
  std::optional<uint32_t> GetTableID() const;

private:
  std::string client_name_;
  int32_t id_;
  uint32_t time_;
  std::optional<uint32_t> table_id_ = std::nullopt;
};

Event ValidateEvent(int32_t id, uint32_t time, std::string body,
                    uint32_t tables_cnt);

std::vector<Event> ReadEvents(std::ifstream &in, uint32_t tables_cnt);

void HandleEvents(std::vector<Event> &events,
                  std::unordered_map<ClientName, Client> &clients,
                  std::vector<Table> &tables, std::queue<ClientName> &queue,
                  uint32_t start_time, uint32_t end_time, uint32_t tables_cnt);

void HandleEventsID1(std::vector<Event> &events,
                     std::unordered_map<ClientName, Client> &clients,
                     std::vector<Table> &tables, std::queue<ClientName> &queue,
                     uint32_t start_time, uint32_t end_time,
                     uint32_t tables_cnt, const Event &event);

void HandleEventsID2(std::vector<Event> &events,
                     std::unordered_map<ClientName, Client> &clients,
                     std::vector<Table> &tables, std::queue<ClientName> &queue,
                     uint32_t start_time, uint32_t end_time,
                     uint32_t tables_cnt, const Event &event);

void HandleEventsID3(std::vector<Event> &events,
                     std::unordered_map<ClientName, Client> &clients,
                     std::vector<Table> &tables, std::queue<ClientName> &queue,
                     uint32_t start_time, uint32_t end_time,
                     uint32_t tables_cnt, const Event &event);

void HandleEventsID4(std::vector<Event> &events,
                     std::unordered_map<ClientName, Client> &clients,
                     std::vector<Table> &tables, std::queue<ClientName> &queue,
                     uint32_t start_time, uint32_t end_time,
                     uint32_t tables_cnt, const Event &event);

void HandleLeaveEvent(std::vector<Event> &events,
                     std::unordered_map<ClientName, Client> &clients,
                     std::vector<Table> &tables, std::queue<ClientName> &queue,
                     uint32_t start_time, uint32_t end_time,
                     uint32_t tables_cnt, const Event &event);

void HandleEndOfDayEvents(std::vector<Event> &events,
                     std::unordered_map<ClientName, Client> &clients,
                     std::vector<Table> &tables, std::queue<ClientName> &queue,
                     uint32_t start_time, uint32_t end_time,
                     uint32_t tables_cnt);
