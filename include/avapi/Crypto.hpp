#ifndef CRYPTO_H
#define CRYPTO_H
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"
#include "avapi/TimeSeries.hpp"
#include "avapi/crypto/ExchangeRate.hpp"
#include "avapi/crypto/HealthIndex.hpp"

namespace avapi {

class Crypto : public ApiCall {
public:
    Crypto();
    explicit Crypto(const std::string &symbol, const std::string &key = "");
    ~Crypto();

    void setApiKey(const std::string &key);
    void setSymbol(const std::string &symbol);
    std::string &Symbol() { return symbol; }

    void setOutputSize(const SeriesSize &size);
    TimeSeries getTimeSeries(const SeriesType &type,
                             const std::string &market = "USD");
    ExchangeRate *Exchange(const std::string &market = "USD");
    HealthIndex *Health();

private:
    std::string symbol;
    ExchangeRate *exchange_rate = nullptr;
    HealthIndex *health_index = nullptr;
    static const std::vector<std::string> series_function;
};

} // namespace avapi
#endif