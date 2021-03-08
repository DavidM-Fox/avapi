#ifndef CRYPTO_H
#define CRYPTO_H
#include <string>
#include "avapi/TimeSeries.hpp"

namespace avapi {

class TimeSeries;
class ExchangeRate;
class ApiCall;

namespace crypto_series {
enum type { INTRADAY, DAILY, WEEKLY, MONTHLY };
}

class Crypto {
public:
    explicit Crypto(const std::string &symbol, const std::string &api_key);

    TimeSeries getTimeSeries(const SeriesType &type,
                             const std::string &market = "USD");

    void setOutputSize(const std::string &size = "compact");

    ExchangeRate getExchangeRate(const std::string &market = "USD");

private:
    ApiCall *m_apiCall = nullptr;
    std::string m_symbol;
    std::string m_outputSize;
    std::string m_apiKey;

    static const std::vector<std::string> m_seriesFunctionStrings;
};
} // namespace avapi
#endif