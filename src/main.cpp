#include <cstdint>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#include "client.h"
#include "events.h"
#include "constants.h"
#include "table.h"
#include "utils.h"


int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Too few args" << std::endl;
    return EXIT_FAILURE;
  }
  std::ifstream in(argv[1]);
  if (!in.is_open()) {
    std::cerr << "No such file" << std::endl;
    return EXIT_FAILURE;
  }
  uint32_t tables_cnt;
  in >> tables_cnt;
  std::string start_time, end_time;
  in >> start_time >> end_time;
  uint32_t start_time_minutes;
  uint32_t end_time_minutes;

  try {
    start_time_minutes = ParseTime(start_time);
  } catch (const std::exception &e) {
    std::cerr << "Incorrect format in start time: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  try {
    end_time_minutes = ParseTime(end_time);
  } catch (const std::exception &e) {
    std::cerr << "Incorrect format in end time: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  uint32_t cost_per_hour;
  in >> cost_per_hour;
  std::string event;
  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::vector<Event> events;
  try {
    events = ReadEvents(in, tables_cnt);
  } catch (std::exception &e) {
    std::cerr << "Incorrect events format " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  std::unordered_map<ClientName, Client> clients;
  std::vector<Table> tables(tables_cnt, Table(cost_per_hour));
  std::queue<ClientName> queue;
  std::cout << GetTimeString(start_time_minutes) << std::endl;
  try {
    HandleEvents(events, clients, tables, queue, start_time_minutes, end_time_minutes, tables_cnt); 
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << GetTimeString(end_time_minutes) << std::endl;
  TablesInfo(tables);
  
}
