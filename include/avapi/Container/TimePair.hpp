#ifndef TIMEPAIR_H
#define TIMEPAIR_H
#include <vector>
#include <iomanip>

namespace avapi {
class TimePair {
public:
    TimePair() : timestamp(0), data({0}) {}

    TimePair(const std::time_t &time, const std::vector<double> &data)
        : timestamp(time), data(data)
    {
    }

    TimePair(const TimePair &pair) : timestamp(pair.timestamp), data(pair.data)
    {
    }

    std::time_t timestamp;
    std::vector<double> data;
    double &operator[](size_t i) { return data[i]; }
};
} // namespace avapi
#endif