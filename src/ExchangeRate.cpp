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
    : m_fromSymbol(from), m_toSymbol(to), m_timestamp(t), m_data(data)
{
}

/// @brief print formatted avapi::ExchangeRate data
void ExchangeRate::printData()
{
    std::cout << m_fromSymbol << " -> " << m_toSymbol << " Exchange Rate\n";

    std::cout << std::setw(15) << std::left << "Timestamp: ";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << m_timestamp << '\n';

    std::cout << std::setw(15) << std::left << "Exchange Rate:";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << m_data[0] << '\n';

    std::cout << std::setw(15) << std::left << "Bid Price:";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << m_data[1] << '\n';

    std::cout << std::setw(15) << std::left << "Ask Price";
    std::cout << std::setw(15) << std::right << std::fixed
              << std::setprecision(2) << m_data[2] << '\n';
}

} // namespace avapi