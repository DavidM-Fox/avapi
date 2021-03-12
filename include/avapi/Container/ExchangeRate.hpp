#ifndef EXCHANGERATE_H
#define EXCHANGERATE_H
#include <iomanip>
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"

namespace avapi {

class ExchangeRate : public ApiCall {
public:
    ExchangeRate(const std::string &from, const std::string &to,
                 const std::string &key);

    std::string from_symbol;
    std::string to_symbol;

    std::time_t &Timestamp() { return timestamp; }
    float &Price() { return exchange_data[0]; }
    float &BidPrice() { return exchange_data[1]; }
    float &AskPrice() { return exchange_data[2]; }
    float &operator[](size_t i) { return exchange_data[i]; }

    void Update();
    void printData();

private:
    std::time_t timestamp;
    std::vector<float> exchange_data;
};

} // namespace avapi
#endif