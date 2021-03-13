#ifndef ANNUALEARNINGS_H
#define ANNUALEARNINGS_H
#include <string>
#include <vector>

namespace avapi {
class AnnualEarnings {
public:
    AnnualEarnings();
    AnnualEarnings(const std::string &symbol);

    std::string symbol;

    struct report {
        std::string fiscal_date_ending;
        std::string reported_eps;
    };

    std::vector<report> data;
    void printData(const size_t &count = 0);
    const report &operator[](size_t i) { return data[i]; }
};

} // namespace avapi
#endif