#include <iostream>
#include <fmt/core.h>
#include "rapidcsv.h"
#include "avapi/ApiCall.hpp"
#include "avapi/misc.hpp"
#include "avapi/Container/GlobalQuote.hpp"

namespace avapi {

/// @brief Print formatted GlobalQuote data
void GlobalQuote::printData()
{

    std::cout << std::string(29, '-') << '\n';
    fmt::print("|{:^27}|\n", symbol + " Global Quote");
    std::cout << std::string(29, '-') << '\n';

    fmt::print("|{:<12}{:>15}|\n", "Timestamp:", timestamp);

    for (size_t i = 0; i < quote_data.size(); ++i) {
        fmt::print("|{:<12}{:>15.2f}|\n", headers[i] + ":", quote_data[i]);
    }
}

/// @brief All GlobalQuote instances have the same headers
const std::vector<std::string> GlobalQuote::headers{
    "Open",   "High",       "Low",    "Close",
    "Volume", "Prev_Close", "Change", "Change%"};

} // namespace avapi
