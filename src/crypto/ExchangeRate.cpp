#include <iostream>
#include "avapi/crypto/ExchangeRate.hpp"

namespace avapi {

/// @brief ExchangeRate constructor
/// @param from Symbol converting from
/// @param to Symbol converting to
/// @param timestamp A Unix timestamp
/// @param data std::vector<float> ordered: [Exchange Rate, Bid Price, Ask
/// Price]
ExchangeRate::ExchangeRate(const std::string &from, const std::string &to,
                           const std::time_t &t, const std::vector<float> &data)
    : from_symbol(from), to_symbol(to), timestamp(t), exchange_data(data)
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
              << std::setprecision(2) << exchange_data[0] << '\n';

    std::cout << std::setw(15) << std::left << "Bid Price:";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << exchange_data[1] << '\n';

    std::cout << std::setw(15) << std::left << "Ask Price";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << exchange_data[2] << '\n';
}

} // namespace avapi