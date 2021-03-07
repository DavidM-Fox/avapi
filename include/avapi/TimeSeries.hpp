#ifndef TIMESERIES_H
#define TIMESERIES_H
#include <vector>
#include <string>
#include "avapi/TimePair.hpp"

namespace avapi {

class TimeSeries {

public:
    enum class type { INTRADAY, DAILY, WEEKLY, MONTHLY };

    TimeSeries(const std::vector<avapi::TimePair> &data);
    TimeSeries();

    void pushBack(const TimePair &pair);

    void setSymbol(const std::string &symbol);
    void setType(const TimeSeries::type &type);
    void setAdjusted(const bool &adjusted);
    void setTitle(const std::string &title);
    void setHeaders(const std::vector<std::string> &headers);
    void reverseData();
    void printData(const size_t &count);

    std::string symbol();

    const size_t rowCount();
    const size_t colCount();

    std::vector<std::string> m_headers;

    TimePair &operator[](size_t i) { return m_data[i]; }
    friend std::ostream &operator<<(std::ostream &os, const TimeSeries &series);

private:
    std::string m_symbol;
    TimeSeries::type m_type;
    std::string m_title;

    bool m_isAdjusted;

    std::vector<TimePair> m_data;
    size_t m_nRows;
    size_t m_nCols;

    std::string m_market;
};

} // namespace avapi
#endif