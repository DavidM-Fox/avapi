#ifndef STOCK_H
#define STOCK_H
#include <string>
#include <vector>
#include "ApiCall.hpp"
#include "TimeSeries.hpp"
#include "GlobalQuote.hpp"

namespace avapi {

class Stock {
public:
    Stock();
    explicit Stock(const std::string &symbol);
    explicit Stock(const std::string &symbol, const std::string &api_key);

    void setOutputSize(const SeriesSize &size);

    TimeSeries getTimeSeries(const SeriesType &type, const bool &adjusted,
                             const std::string &interval = "30min");
    GlobalQuote getGlobalQuote();

    std::string symbol;

private:
    ApiCall api_call;
    static const std::vector<std::string> series_function;
};
} // namespace avapi
#endif