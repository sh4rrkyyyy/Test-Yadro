#pragma once
#include <cstdint>
#include <optional>
#include <string>
using ClientName = std::string;
class Client {
public:
  Client() = default;
  Client(const std::string &name, uint32_t table);
  Client(std::string &&name, uint32_t table);
  Client(const std::string &name);
  Client(std::string &&name);

  bool InClub() const { return in_club_; }
  void SetTableID(uint32_t table);
  std::optional<uint32_t> GetTableID() const;

private:
  std::string name_;
  std::optional<uint32_t> table_id_ = std::nullopt;
  bool in_club_ = false;
};
