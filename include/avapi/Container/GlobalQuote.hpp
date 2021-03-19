#ifndef GLOBALQUOTE_H
#define GLOBALQUOTE_H
#include <iomanip>
#include <string>
#include <vector>

namespace avapi {

class GlobalQuote {
public:
    GlobalQuote() : symbol(""), timestamp(0), quote_data() {}
    GlobalQuote(const std::string &symbol, const std::time_t &timestamp,
                const std::vector<float> &data)
        : symbol(symbol), timestamp(timestamp), quote_data(data)
    {
    }

    std::string symbol;
    std::time_t timestamp;

    /// @brief [Open, High, Low, Close, Volume, PreviousClose, ChangeAmount,
    /// ChangePercent]
    std::vector<float> quote_data;
    float &operator[](size_t i) { return quote_data[i]; }
    void printData();

private:
    // All GlobalQuotes have the same headers
    static const std::vector<std::string> headers;
};

} // namespace avapi

#endif