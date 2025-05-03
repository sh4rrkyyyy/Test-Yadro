#include "client.h"
#include <optional>


Client::Client(const std::string &name) : name_(name), in_club_(true) {}

Client::Client(std::string &&name) : name_(std::move(name)), in_club_(true) {}

void Client::SetTableID(uint32_t table_id) {
  table_id_ = table_id;
}

std::optional<uint32_t> Client::GetTableID() const {
  return table_id_;
}