#ifndef CRYPTO_H
#define CRYPTO_H
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"
#include "avapi/TimeSeries.hpp"
#include "avapi/crypto/ExchangeRate.hpp"

namespace avapi {

class Crypto {
public:
    Crypto();
    explicit Crypto(const std::string &symbol);
    explicit Crypto(const std::string &symbol, const std::string &api_key);

    void setOutputSize(const SeriesSize &size);

    TimeSeries getTimeSeries(const SeriesType &type,
                             const std::string &market = "USD");
    ExchangeRate getExchangeRate(const std::string &market = "USD");

    std::string symbol;

private:
    ApiCall api_call;
    static const std::vector<std::string> series_function;
};
} // namespace avapi
#endif