#ifndef GLOBALQUOTE_H
#define GLOBALQUOTE_H
#include <iomanip>
#include <string>
#include <vector>
#include "avapi/ApiCall.hpp"

namespace avapi {
class GlobalQuote : public ApiCall {
public:
    GlobalQuote(const std::string &symbol, const std::string &key);

    std::string symbol;

    std::time_t &Timestamp() { return timestamp; }
    float &Open() { return quote_data[0]; }
    float &High() { return quote_data[1]; }
    float &Low() { return quote_data[2]; }
    float &Close() { return quote_data[3]; }
    float &Volume() { return quote_data[4]; }
    float &PreviousClose() { return quote_data[5]; }
    float &ChangeAmount() { return quote_data[6]; }
    float &ChangePercent() { return quote_data[7]; }
    float &operator[](size_t i) { return quote_data[i]; }

    void Update();
    void printData();

private:
    std::time_t timestamp;
    std::vector<float> quote_data;

    // All GlobalQuotes have the same headers
    static const std::vector<std::string> headers;
};
} // namespace avapi

#endif