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
    const std::string &FromSymbol() const { return m_fromSymbol; }
    const std::string &ToSymbol() const { return m_toSymbol; }
    const std::time_t &Timestamp() const { return m_timestamp; }
    const float &Exchange() const { return m_data[0]; }
    const float &BidPrice() const { return m_data[1]; }
    const float &AskPrice() const { return m_data[2]; }

    const float &operator[](size_t i) { return m_data[i]; }

private:
    const std::string m_fromSymbol;
    const std::string m_toSymbol;
    const std::time_t m_timestamp;
    const std::vector<float> m_data;
};

} // namespace avapi
#endif