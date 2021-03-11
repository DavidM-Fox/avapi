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
    explicit Crypto(const std::string &symbol, const std::string &key = "");
    ~Crypto();

    void setApiKey(const std::string &key);
    void setSymbol(const std::string &symbol);
    void setOutputSize(const SeriesSize &size);

    TimeSeries getTimeSeries(const SeriesType &type,
                             const std::string &market = "USD");

    ExchangeRate *Exchange(const std::string &market = "USD");
    std::string symbol;

private:
    std::string api_key;
    ApiCall api_call;

    ExchangeRate *exchange_rate = nullptr;
    static const std::vector<std::string> series_function;
};

} // namespace avapi
#endif