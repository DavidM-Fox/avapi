#ifndef EXCHANGERATE_H
#define EXCHANGERATE_H

#include <string>
#include <iomanip>
#include <vector>

namespace avapi {

class ExchangeRate {
public:
    ExchangeRate(const std::string &from, const std::string &to,
                 const std::time_t &t, const std::vector<float> &data);
    const std::string from_symbol;
    const std::string to_symbol;
    const std::time_t timestamp;
    const float exchange_rate;
    const float bid_price;
    const float ask_price;
    void printData();
};

} // namespace avapi
#endif