#ifndef COMPANYEARNINGS_H
#define COMPANYEARNINGS_H
#include <string>
#include "avapi/ApiCall.hpp"

namespace avapi {

class CompanyEarnings : public ApiCall {
public:
    CompanyEarnings(const std::string &symbol, const std::string &key);
    ~CompanyEarnings();

    std::string symbol;

    class AnnualEarnings;
    class QuarterlyEarnings;

    AnnualEarnings *Annual() { return annual_earnings; }
    QuarterlyEarnings *Quarterly() { return quarterly_earnings; };
    void Update();

    class AnnualEarnings {
    public:
        AnnualEarnings(const std::string &symbol) : symbol(symbol) {}

        struct report {
            std::string fiscal_date_ending;
            std::string reported_eps;
        };

        std::vector<report> data;
        void printData();
        const report &operator[](size_t i) { return data[i]; }

    private:
        std::string symbol;
    };

    class QuarterlyEarnings {
    public:
        QuarterlyEarnings(const std::string &symbol) : symbol(symbol) {}

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

    private:
        std::string symbol;
    };

private:
    AnnualEarnings *annual_earnings;
    QuarterlyEarnings *quarterly_earnings;
};

} // namespace avapi
#endif