#ifndef TIMESERIES_H
#define TIMESERIES_H
#include <string>
#include <vector>
#include "avapi/TimePair.hpp"

namespace avapi {

enum class SeriesSize { COMPACT = 0, FULL };
enum class SeriesType { INTRADAY = 0, DAILY, WEEKLY, MONTHLY };

class TimeSeries {
public:
    TimeSeries();
    TimeSeries(const std::vector<avapi::TimePair> &data);

    void pushBack(const TimePair &pair);
    void reverseData();
    void printData(const size_t &count);
    void setHeaders(const std::vector<std::string> &headers);

    const size_t rowCount();
    const size_t colCount();

    std::string m_symbol;
    SeriesType m_type;
    bool m_adjusted;
    std::string m_market;

    std::string m_title;
    std::vector<std::string> m_headers;

    TimePair &operator[](size_t i) { return m_data[i]; }
    friend std::ostream &operator<<(std::ostream &os, const TimeSeries &series);

private:
    std::vector<TimePair> m_data;
    size_t m_nRows;
    size_t m_nCols;
};

TimeSeries parseCsvFile(const std::string &file, const bool &crypto = false);
TimeSeries parseCsvString(const std::string &data, const bool &crypto = false);

std::time_t toUnixTimestamp(const std::string &input);
bool isJsonString(const std::string &data);

} // namespace avapi
#endif