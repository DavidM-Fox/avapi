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

/// @brief Print formatted QuarterlyEarnings data
void QuarterlyEarnings::printData()
{
    std::cout << std::string(102, '-') << '\n';
    fmt::print("|{:^100}|\n", symbol + " Quarterly Earnings");
    fmt::print("|{:^20}|{:^15}|{:^15}|{:^15}|{:^15}|{:^15}|\n",
               "Fiscal Date Ending", "Reported Date", "Reported EPS",
               "Estimated EPS", "Surprise", "Surprise %");
    std::cout << std::string(102, '-') << '\n';

    for (auto &report : data) {
        if (report.estimated_eps == "None") {
            fmt::print("|{:^20}|{:^15}|{:>15.2f}|{:>15}|{:>15}|{:>15}|\n",
                       report.fiscal_date_ending, report.reported_date,
                       std::stof(report.reported_eps), "None", "None", "None");
        }
        else {
            fmt::print(
                "|{:^20}|{:^15}|{:>15.2f}|{:>15.2f}|{:>15.2f}|{:>15.2f}|\n",
                report.fiscal_date_ending, report.reported_date,
                std::stof(report.reported_eps), std::stof(report.estimated_eps),
                std::stof(report.surprise),
                std::stof(report.surprise_percentage));
        }
    }
}

} // namespace avapi