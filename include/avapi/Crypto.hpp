#ifndef CRYPTO_H
#define CRYPTO_H
#include <string>
#include <vector>
#include "ApiCall.hpp"
#include "TimeSeries.hpp"
#include "ExchangeRate.hpp"

namespace avapi {

class ApiCall;

class Crypto {
public:
    // Constructors
    Crypto();
    explicit Crypto(const std::string &symbol);
    explicit Crypto(const std::string &symbol, const std::string &api_key);

    // Historical Data
    TimeSeries getTimeSeries(const SeriesType &type,
                             const std::string &market = "USD");
    ExchangeRate getExchangeRate(const std::string &market = "USD");

    std::string m_symbol;

private:
    ApiCall m_apiCall;
    static const std::vector<std::string> m_seriesFunctionStrings;
};
} // namespace avapi
#endif