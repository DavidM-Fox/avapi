#ifndef EXCHANGERATE_H
#define EXCHANGERATE_H
#include <iomanip>
#include <string>
#include <vector>

namespace avapi {

class ExchangeRate {
public:
    ExchangeRate(const std::string &from, const std::string &to,
                 const std::time_t &t, const std::vector<float> &data);

    void printData();

    // Easy Getters
    const std::string &FromSymbol() const { return from_symbol; }
    const std::string &ToSymbol() const { return to_symbol; }
    const std::time_t &Timestamp() const { return timestamp; }
    const float &Exchange() const { return exchange_data[0]; }
    const float &BidPrice() const { return exchange_data[1]; }
    const float &AskPrice() const { return exchange_data[2]; }

    const float &operator[](size_t i) { return exchange_data[i]; }

private:
    const std::string from_symbol;
    const std::string to_symbol;
    const std::time_t timestamp;
    const std::vector<float> exchange_data;
};

} // namespace avapi
#endif