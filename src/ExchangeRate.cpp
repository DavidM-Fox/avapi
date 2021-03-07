#include <iostream>
#include "avapi/ExchangeRate.hpp"

namespace avapi {

/// @brief ExchangeRate constructor
/// @param from Symbol converting from
/// @param to Symbol converting to
/// @param timestamp A Unix timestamp
/// @param data std::vector<float> ordered: [Exchange Rate, Bid Price, Ask
/// Price]
ExchangeRate::ExchangeRate(const std::string &from, const std::string &to,
                           const std::time_t &t, const std::vector<float> &data)
    : from_symbol(from), to_symbol(to), timestamp(t), exchange_rate(data[0]),
      bid_price(data[1]), ask_price(data[2])
{
}

/// @brief print formatted avapi::ExchangeRate data
void ExchangeRate::printData()
{
    std::cout << from_symbol << " -> " << to_symbol << " Exchange Rate\n";

    std::cout << std::setw(15) << std::left << "Timestamp: ";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << timestamp << '\n';

    std::cout << std::setw(15) << std::left << "Exchange Rate:";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << exchange_rate << '\n';

    std::cout << std::setw(15) << std::left << "Bid Price:";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << bid_price << '\n';

    std::cout << std::setw(15) << std::left << "Ask Price";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << ask_price << '\n';
}

} // namespace avapi