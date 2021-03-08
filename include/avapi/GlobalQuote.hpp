#ifndef GLOBALQUOTE_H
#define GLOBALQUOTE_H
#include <iomanip>
#include <string>
#include <vector>

namespace avapi {

class GlobalQuote {
public:
    GlobalQuote(const std::string &symbol, const std::time_t &timestamp,
                const std::vector<float> &data);

    void printData();

    // Easy Getters
    const std::string &Symbol() const { return m_symbol; }
    const std::time_t &Timestamp() const { return m_timestamp; }
    const float &Open() const { return m_data[0]; }
    const float &High() const { return m_data[1]; }
    const float &Low() const { return m_data[2]; }
    const float &Close() const { return m_data[3]; }
    const float &Volume() const { return m_data[4]; }
    const float &PreviousClose() const { return m_data[5]; }
    const float &ChangeAmount() const { return m_data[6]; }
    const float &ChangePercent() const { return m_data[7]; }

    const float &operator[](size_t i) { return m_data[i]; }

private:
    const std::string m_symbol;
    const std::time_t m_timestamp;
    const std::vector<float> m_data;

    // All GlobalQuotes have the same headers
    static const std::vector<std::string> m_headers;
};

} // namespace avapi
#endif