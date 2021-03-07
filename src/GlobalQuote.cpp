#include "avapi/GlobalQuote.hpp"
#include <iostream>

namespace avapi {

/// @brief GlobalQuote constructor
/// @param timestamp A Unix timestamp
/// @param data std::vector<float> ordered: [Open, High, Low, Close, Volume,
/// Prev_Close, Change, Change%]
GlobalQuote::GlobalQuote(const std::string &symbol,
                         const std::time_t &timestamp,
                         const std::vector<float> &data)
    : symbol(symbol), timestamp(timestamp), open(data[0]), high(data[1]),
      low(data[2]), close(data[3]), volume(data[4]), close_previous(data[5]),
      change(data[6]), change_percent(data[7]),
      m_headers({"Open", "High", "Low", "Close", "Volume", "Prev_Close",
                 "Change", "Change%"}),
      m_data(data)
{
}

/// @brief print formatted avapi::GlobalQuote data
void GlobalQuote::printData()
{
    std::cout << std::setw(12) << std::left << "Timestamp:";
    std::cout << std::setw(15) << std::right << timestamp << '\n';
    for (size_t i = 0; i < m_data.size(); ++i) {
        std::string header = m_headers[i] + ":";
        std::cout << std::setw(12) << std::left << header;
        std::cout << std::setw(15) << std::right << std::fixed
                  << std::setprecision(2) << m_data[i];
        std::cout << '\n';
    }
}

} // namespace avapi