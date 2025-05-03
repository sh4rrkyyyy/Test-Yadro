#pragma once
#include <cstdint>
#include <string>

namespace constants {
inline const std::string NameSymbols = "abcdefghijklmnopqrstuvwxyz0123456789_";
inline constexpr int32_t ErrorEventID = 13;
inline constexpr int32_t LeaveEventID = 11;
inline constexpr int32_t SeatEventID = 12;
inline const std::string ErrorAlreadyInClub = "YouShallNotPass";
inline const std::string ErrorNotOpen = "NotOpenYet";
inline const std::string ErrorNotInClub = "ClientUnknown";
inline const std::string ErrorOccupiedTable = "PlaceIsBusy";
inline const std::string ErrorExistFreeTables = "ICanWaitNoLonger!";

}
