#ifndef EXCHANGERATE_H
#define EXCHANGERATE_H
#include <iomanip>
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"

namespace avapi {

class ExchangeRate {
public:
    ExchangeRate(const std::string &from, const std::string &to,
                 const std::string &api_key);

    void setApiKey(const std::string &key) { api_call.api_key = key; }
    void setSymbol(const std::string &symbol) { this->from_symbol = symbol; }

    std::string from_symbol;
    std::string to_symbol;
    std::time_t timestamp;

    std::vector<float> exchange_data;
    void printData();

    float &Price() { return exchange_data[0]; }
    float &BidPrice() { return exchange_data[1]; }
    float &AskPrice() { return exchange_data[2]; }
    float &operator[](size_t i) { return exchange_data[i]; }

private:
    ApiCall api_call;
    void updateExchangeRate();
};

} // namespace avapi
#endif