#include "table.h"
#include "utils.h"
#include <iostream>
bool Table::Occupied() const { return occupied_; }

void Table::SetStartMinutes(uint32_t start_minutes) {
  start_minutes_ = start_minutes;
}

void Table::UpdateTotalMinutesAndRevenue(uint32_t end_minutes) {
  total_minutes_ += (end_minutes - start_minutes_);
  updateRevenue(end_minutes);
  occupied_ = false;
}

void Table::updateRevenue(uint32_t end_minutes) {
  auto minutes = end_minutes - start_minutes_;
  revenue_ += (minutes % 60 == 0 ? minutes / 60 : minutes / 60 + 1);
}

uint32_t Table::GetTotalMinutes() const { return total_minutes_; }

uint32_t Table::GetRevenue() const { return revenue_; }
void TablesInfo(std::vector<Table> &tables) {
  for (size_t i = 0; i < tables.size(); ++i) {
    std::cout << i + 1 << ' ' << tables[i].GetRevenue() << ' '
              << GetTimeString(tables[i].GetTotalMinutes()) << std::endl;
  }
}
