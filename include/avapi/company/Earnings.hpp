#ifndef COMPANYEARNINGS_H
#define COMPANYEARNINGS_H
#include "avapi/ApiCall.hpp"
#include <string>

namespace avapi {

class CompanyEarnings {
public:
    CompanyEarnings(const std::string &symbol, const std::string &api_key);
    ~CompanyEarnings();

    void setApiKey(const std::string &api_key);
    void printAnnualData();

    std::string symbol;

    struct AnnualEarning {
        std::string fiscal_date_ending;
        std::string reported_eps;
    };

    struct QuarterlyEarning {
        std::string fiscal_date_ending;
        std::string reported_date;
        std::string reported_eps;
        std::string estimated_eps;
        std::string surprise;
        std::string surprise_percentage;
    };

    std::vector<AnnualEarning> &Annual() { return annual_earnings; }
    std::vector<QuarterlyEarning> &Quarterly() { return quarterly_earnings; };

private:
    ApiCall api_call;

    void updateEarnings();
    std::vector<AnnualEarning> annual_earnings;
    std::vector<QuarterlyEarning> quarterly_earnings;
};

} // namespace avapi
#endif