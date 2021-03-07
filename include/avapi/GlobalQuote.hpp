#ifndef GLOBALQUOTE_H
#define GLOBALQUOTE_H
#include <string>
#include <vector>
#include <iomanip>

namespace avapi {

class GlobalQuote {
public:
    GlobalQuote(const std::string &symbol, const std::time_t &timestamp,
                const std::vector<float> &data);

    const std::time_t timestamp;
    const std::string symbol;
    const float open;
    const float high;
    const float low;
    const float close;
    const float volume;
    const float close_previous;
    const float change;
    const float change_percent;
    void printData();

private:
    const std::vector<float> m_data;
    const std::vector<std::string> m_headers;
};

} // namespace avapi
#endif