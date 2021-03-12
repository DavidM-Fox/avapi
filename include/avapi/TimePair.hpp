#ifndef TIMEPAIR_H
#define TIMEPAIR_H
#include <vector>
#include <iomanip>

namespace avapi {
class TimePair {
public:
    TimePair(const std::time_t &time, const std::vector<float> &data)
        : timestamp(time), data(data)
    {
    }
    TimePair(const TimePair &pair) : timestamp(pair.timestamp), data(pair.data)
    {
    }

    std::time_t timestamp;
    std::vector<float> data;
    float &operator[](size_t i) { return data[i]; }
};
} // namespace avapi
#endif