#ifndef COMPANYEARNINGS_H
#define COMPANYEARNINGS_H
#include <string>
#include "avapi/ApiCall.hpp"
#include "avapi/Container/AnnualEarnings.hpp"
#include "avapi/Container/QuarterlyEarnings.hpp"

namespace avapi {

class CompanyEarnings : public ApiCall {
public:
    CompanyEarnings();
    explicit CompanyEarnings(const std::string &symbol,
                             const std::string &key = "");

    std::string symbol;

    AnnualEarnings annual() { return annual_earnings; }
    QuarterlyEarnings quarterly() { return quarterly_earnings; };
    void update();

private:
    AnnualEarnings annual_earnings;
    QuarterlyEarnings quarterly_earnings;
};

} // namespace avapi
#endif