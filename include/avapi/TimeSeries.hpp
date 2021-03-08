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

    std::string symbol;
    SeriesType type;
    bool is_adjusted;
    std::string market;

    std::string title;
    std::vector<std::string> m_headers;

    TimePair &operator[](size_t i) { return data_series[i]; }
    friend std::ostream &operator<<(std::ostream &os, const TimeSeries &series);

private:
    std::vector<TimePair> data_series;
    size_t n_rows;
    size_t n_cols;
};

TimeSeries parseCsvFile(const std::string &file, const bool &crypto = false);
TimeSeries parseCsvString(const std::string &data, const bool &crypto = false);

std::time_t toUnixTimestamp(const std::string &input);
bool isJsonString(const std::string &data);

} // namespace avapi
#endif