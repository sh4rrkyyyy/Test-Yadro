#pragma once
#include <cstdint>
#include <vector>

class Table {
public:
 bool Occupied() const;
 void SetStartMinutes(uint32_t start_minutes);
 void UpdateTotalMinutesAndRevenue(uint32_t end_minutes);
 uint32_t GetTotalMinutes() const;
 uint32_t GetRevenue() const;
private:
  void updateRevenue(uint32_t end_minutes);
  bool occupied_ = false;
  uint32_t total_minutes_ = 0;
  uint32_t start_minutes_;
  uint32_t revenue_;
};

void TablesInfo(std::vector<Table> &tables);