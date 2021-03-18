#include <iostream>
#include <fmt/core.h>
#include "avapi/Container/AnnualEarnings.hpp"

namespace avapi {

/// @brief Default constructor
AnnualEarnings::AnnualEarnings() : symbol("") {}

/// @brief Constructor
/// @param symbol: The stock symbol e.g. "TSLA"
AnnualEarnings::AnnualEarnings(const std::string &symbol) : symbol(symbol) {}

/// @brief Print AnnualEarnings data
/// @param count: The number of rows to print (default = 0 or all)
void AnnualEarnings::printData(const size_t &count)
{
    size_t n = count;
    size_t n_rows = data.size();
    if (count > n_rows || count == 0)
        n = n_rows;

    std::cout << std::string(38, '-') << '\n';
    fmt::print("|{:^36}|\n", symbol + " Annual Earnings");
    fmt::print("|{:^20}|{:^15}|\n", "Fiscal Date Ending", "Reported EPS");
    std::cout << std::string(38, '-') << '\n';

    for (size_t i = 0; i < n; ++i) {
        fmt::print("|{:^20}|{:>15.2f}|\n", data[i].fiscal_date_ending,
                   std::stof(data[i].reported_eps));
    }
}

} // namespace avapi