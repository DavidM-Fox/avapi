#include <iostream>
#include <fmt/core.h>
#include "avapi/Container/QuarterlyEarnings.hpp"

namespace avapi {

/// @brief Default constructor
QuarterlyEarnings::QuarterlyEarnings() : symbol("") {}

/// @brief Constructor
/// @param symbol: The stock symbol e.g. "TSLA"
QuarterlyEarnings::QuarterlyEarnings(const std::string &symbol) : symbol(symbol)
{
}

/// @brief Print QuarterlyEarnings data
/// @param count: The number of rows to print (default = 0 or all)
void QuarterlyEarnings::printData(const size_t &count)
{
    size_t n = count;
    size_t n_rows = data.size();
    if (count > n_rows || count == 0)
        n = n_rows;

    std::cout << std::string(102, '-') << '\n';
    fmt::print("|{:^100}|\n", symbol + " Quarterly Earnings");
    fmt::print("|{:^20}|{:^15}|{:^15}|{:^15}|{:^15}|{:^15}|\n",
               "Fiscal Date Ending", "Reported Date", "Reported EPS",
               "Estimated EPS", "Surprise", "Surprise %");
    std::cout << std::string(102, '-') << '\n';

    for (size_t i = 0; i < n; ++i) {
        if (data[i].estimated_eps == "None") {
            fmt::print("|{:^20}|{:^15}|{:>15.2f}|{:>15}|{:>15}|{:>15}|\n",
                       data[i].fiscal_date_ending, data[i].reported_date,
                       std::stof(data[i].reported_eps), "None", "None", "None");
        }
        else {
            fmt::print(
                "|{:^20}|{:^15}|{:>15.2f}|{:>15.2f}|{:>15.2f}|{:>15.2f}|\n",
                data[i].fiscal_date_ending, data[i].reported_date,
                std::stof(data[i].reported_eps),
                std::stof(data[i].estimated_eps), std::stof(data[i].surprise),
                std::stof(data[i].surprise_percentage));
        }
    }
}

} // namespace avapi