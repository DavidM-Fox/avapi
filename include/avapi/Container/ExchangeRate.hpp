#ifndef EXCHANGERATE_H
#define EXCHANGERATE_H
#include <iomanip>
#include <string>
#include <vector>

namespace avapi {

class ExchangeRate {
public:
    ExchangeRate()
        : from_symbol(""), to_symbol(""), timestamp(0), exchange_data()
    {
    }
    ExchangeRate(const std::string &from, const std::string &to,
                 const std::time_t &time, const std::vector<double> &data)
        : from_symbol(from), to_symbol(to), timestamp(time), exchange_data(data)
    {
    }

    std::string from_symbol;
    std::string to_symbol;
    std::time_t timestamp;

    /// @brief [exchange_rate, bid_price, ask_price]
    std::vector<double> exchange_data;
    double &operator[](size_t i) { return exchange_data[i]; }
    void printData();
};

} // namespace avapi
#endif