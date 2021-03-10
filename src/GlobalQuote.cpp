#include "avapi/company/GlobalQuote.hpp"
#include <iostream>

namespace avapi {

/// @brief GlobalQuote constructor
/// @param timestamp A Unix timestamp
/// @param data std::vector<float> ordered: [Open, High, Low, Close, Volume,
/// Prev_Close, Change, Change%]
GlobalQuote::GlobalQuote(const std::string &symbol,
                         const std::time_t &timestamp,
                         const std::vector<float> &data)
    : symbol(symbol), timestamp(timestamp), quote_data(data)
{
}

/// @brief print formatted avapi::GlobalQuote data
void GlobalQuote::printData()
{
    std::cout << std::setw(12) << std::left << "Timestamp:";
    std::cout << std::setw(15) << std::right << timestamp << '\n';
    for (size_t i = 0; i < quote_data.size(); ++i) {
        std::string header = headers[i] + ":";
        std::cout << std::setw(12) << std::left << header;
        std::cout << std::setw(15) << std::right << std::fixed
                  << std::setprecision(2) << quote_data[i];
        std::cout << '\n';
    }
}

const std::vector<std::string> GlobalQuote::headers{
    "Open",   "High",       "Low",    "Close",
    "Volume", "Prev_Close", "Change", "Change%"};

} // namespace avapi