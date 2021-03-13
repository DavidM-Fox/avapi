#include <iostream>
#include <fmt/core.h>
#include "avapi/Container/AnnualEarnings.hpp"

namespace avapi {

/// @brief Default constructor
AnnualEarnings::AnnualEarnings() : symbol("") {}

/// @brief Constructor
/// @param symbol: The stock symbol e.g. "TSLA"
AnnualEarnings::AnnualEarnings(const std::string &symbol) : symbol(symbol) {}

/// @brief Print formatted annual earnings
void AnnualEarnings::printData()
{
    std::cout << std::string(38, '-') << '\n';
    fmt::print("|{:^36}|\n", symbol + " Annual Earnings");
    fmt::print("|{:^20}|{:^15}|\n", "Fiscal Date Ending", "Reported EPS");
    std::cout << std::string(38, '-') << '\n';

    for (auto &report : data) {
        fmt::print("|{:^20}|{:>15.2f}|\n", report.fiscal_date_ending,
                   std::stof(report.reported_eps));
    }
}

} // namespace avapi