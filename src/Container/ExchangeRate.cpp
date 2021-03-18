#include <iostream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "avapi/misc.hpp"
#include "avapi/Container/ExchangeRate.hpp"

namespace avapi {

/// @brief Print formatted ExchangeRate data
void ExchangeRate::printData()
{
    std::cout << std::string(30, '-') << '\n';
    fmt::print("|{:^28}|\n", "Exchange Rate");
    fmt::print("|{:^28}|\n", from_symbol + " -> " + to_symbol);
    std::cout << std::string(30, '-') << '\n';
    fmt::print("|{:<16}{:>12}|\n", "Timestamp:", timestamp);
    fmt::print("|{:<16}{:>12.2f}|\n", "Exchange Rate:", exchange_data[0]);
    fmt::print("|{:<16}{:>12.2f}|\n", "Bid Price:", exchange_data[1]);
    fmt::print("|{:<16}{:>12.2f}|\n", "Ask Price:", exchange_data[2]);
}

} // namespace avapi