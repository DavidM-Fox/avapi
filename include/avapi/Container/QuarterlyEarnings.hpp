#ifndef QUARTERLYEARNINGS_H
#define QUARTERLYEARNINGS_H
#include <string>
#include <vector>

namespace avapi {

class QuarterlyEarnings {
public:
    QuarterlyEarnings();
    QuarterlyEarnings(const std::string &symbol);

    std::string symbol;

    struct report {
        std::string fiscal_date_ending;
        std::string reported_date;
        std::string reported_eps;
        std::string estimated_eps;
        std::string surprise;
        std::string surprise_percentage;
    };

    std::vector<report> data;
    void printData();
    const report &operator[](size_t i) { return data[i]; }
};

} // namespace avapi
#endif