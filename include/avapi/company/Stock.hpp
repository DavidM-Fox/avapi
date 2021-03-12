#ifndef STOCK_H
#define STOCK_H
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"
#include "avapi/containers/TimeSeries.hpp"

namespace avapi {

class CompanyStock : public ApiCall {
public:
    CompanyStock();
    explicit CompanyStock(const std::string &symbol);
    explicit CompanyStock(const std::string &symbol,
                          const std::string &api_key);

    std::string symbol;

    void setOutputSize(const SeriesSize &size);
    TimeSeries getTimeSeries(const SeriesType &type, const bool &adjusted,
                             const std::string &interval = "30min");

    class GlobalQuote {
    public:
        GlobalQuote();
        GlobalQuote(const std::string &symbol, const std::time_t &timestamp,
                    const std::vector<float> &data);

        void printData();

        // Easy Getters
        const std::string &Symbol() const { return symbol; }
        const std::time_t &Timestamp() const { return timestamp; }
        const float &Open() const { return quote_data[0]; }
        const float &High() const { return quote_data[1]; }
        const float &Low() const { return quote_data[2]; }
        const float &Close() const { return quote_data[3]; }
        const float &Volume() const { return quote_data[4]; }
        const float &PreviousClose() const { return quote_data[5]; }
        const float &ChangeAmount() const { return quote_data[6]; }
        const float &ChangePercent() const { return quote_data[7]; }

        const float &operator[](size_t i) { return quote_data[i]; }

    private:
        const std::string symbol;
        const std::time_t timestamp;
        const std::vector<float> quote_data;

        // All GlobalQuotes have the same headers
        static const std::vector<std::string> headers;
    };

    GlobalQuote getGlobalQuote();

private:
    static const std::vector<std::string> series_function;
};

} // namespace avapi
#endif