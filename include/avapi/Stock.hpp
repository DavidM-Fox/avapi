#ifndef STOCK_H
#define STOCK_H
#include <string>
#include <vector>
#include "avapi/TimeSeries.hpp"

namespace avapi {

class GlobalQuote;
class ApiCall;

class Stock {
public:
    explicit Stock(const std::string &symbol, const std::string &api_key);

    TimeSeries getTimeSeries(const SeriesType &type, const bool &adjusted,
                             const std::string &interval = "30min");
    void setOutputSize(const std::string &size = "compact");
    GlobalQuote getGlobalQuote();

    // Getters
    const std::string &Symbol() const;
    const std::string &ApiKey() const;

private:
    ApiCall *m_apiCall = nullptr;
    std::string m_outputSize;
    std::string m_symbol;
    std::string m_apiKey;

    static const std::vector<std::string> m_seriesFunctionStrings;
};
} // namespace avapi
#endif