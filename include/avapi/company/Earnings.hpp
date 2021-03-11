#ifndef COMPANYEARNINGS_H
#define COMPANYEARNINGS_H
#include "avapi/ApiCall.hpp"
#include <string>

namespace avapi {

class CompanyEarnings {
public:
    CompanyEarnings(const std::string &symbol, const std::string &api_key);
    ~CompanyEarnings();

    void setSymbol(const std::string &symbol) { this->symbol = symbol; }
    void setApiKey(const std::string &key) { api_call.api_key = key; }

    std::string symbol;

    class AnnualEarnings {
    public:
        AnnualEarnings(const std::string &symbol) : symbol(symbol) {}

        struct report {
            std::string fiscal_date_ending;
            std::string reported_eps;
        };

        std::vector<report> data;
        void printData();
        const avapi::CompanyEarnings::AnnualEarnings::report &
        operator[](size_t i) const
        {
            return data[i];
        }

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

    AnnualEarnings *Annual() { return annual_earnings; }
    QuarterlyEarnings *Quarterly() { return quarterly_earnings; };

private:
    ApiCall api_call;

    void updateEarnings();
    AnnualEarnings *annual_earnings;
    QuarterlyEarnings *quarterly_earnings;
};

} // namespace avapi
#endif