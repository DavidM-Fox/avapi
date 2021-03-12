#ifndef CRYPTOPRICING_H
#define CRYPTOPRICING_H
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"
#include "avapi/Container/TimeSeries.hpp"
#include "avapi/Container/ExchangeRate.hpp"

namespace avapi {
class CryptoPricing : public ApiCall {
public:
    CryptoPricing();
    explicit CryptoPricing(const std::string &symbol,
                           const std::string &key = "");

    std::string symbol;

    void setOutputSize(const SeriesSize &size);
    TimeSeries getTimeSeries(const SeriesType &type,
                             const std::string &market = "USD");
    ExchangeRate Exchange(const std::string &market = "USD");

private:
    static const std::vector<std::string> series_function;
};

} // namespace avapi
#endif