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
    TimeSeries(const TimeSeries &series);

    void pushBack(const TimePair &pair);
    void reverseData();
    void printData(const size_t &count = 0);
    void setHeaders(const std::vector<std::string> &headers);

    size_t rowCount();
    size_t colCount();

    std::string symbol;
    SeriesType type;
    bool is_adjusted;
    std::string market;

    std::string title;
    std::vector<std::string> headers;

    TimePair &operator[](size_t i) { return data_series[i]; }
    friend std::ostream &operator<<(std::ostream &os, const TimeSeries &series);

private:
    std::vector<TimePair> data_series;
};

TimeSeries parseCsvString(const std::string &data, const bool &crypto = false);
TimeSeries parseCsvFile(const std::string &file_path,
                        const bool &crypto = false);

} // namespace avapi
#endif