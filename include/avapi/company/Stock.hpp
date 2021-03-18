#ifndef STOCK_H
#define STOCK_H
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"
#include "avapi/Container/TimeSeries.hpp"
#include "avapi/Container/GlobalQuote.hpp"

namespace avapi {

class CompanyStock : public ApiCall {
public:
    CompanyStock();
    explicit CompanyStock(const std::string &symbol,
                          const std::string &key = "");

    std::string symbol;

    TimeSeries getTimeSeries(const SeriesType &type, const bool &adjusted,
                             const std::string &interval = "30min");
    GlobalQuote getGlobalQuote();

private:
    static const std::vector<std::string> series_function;
};
} // namespace avapi
#endif