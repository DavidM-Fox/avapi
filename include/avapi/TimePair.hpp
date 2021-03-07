#ifndef TIMEPAIR_H
#define TIMEPAIR_H
#include <vector>
#include <iomanip>

namespace avapi {
class TimePair {
public:
    TimePair(const std::time_t &time, const std::vector<float> &data)
        : m_time(time), m_data(data)
    {
    }

    std::time_t m_time;
    std::vector<float> m_data;
    float &operator[](size_t i) { return m_data[i]; }
};
} // namespace avapi
#endif