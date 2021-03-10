#ifndef COMPANY_H
#define COMPANY_H
#include "avapi/company/Stock.hpp"
#include "avapi/company/Overview.hpp"
#include "avapi/company/Earnings.hpp"

namespace avapi {

class ApiCall;

class Company {
public:
    Company();
    explicit Company(const std::string &symbol,
                     const std::string &api_key = "");
    ~Company();

    void setApiKey(const std::string &api_key);
    void setSymbol(const std::string &symbol);

    ApiCall getCall() { return api_call; }

    CompanyStock *Stock();
    CompanyOverview *Overview(const bool &update = false);
    CompanyEarnings *Earnings(const bool &update = false);

    std::string symbol;

private:
    ApiCall api_call;

    CompanyStock *company_stock = nullptr;
    CompanyOverview *company_overview = nullptr;
    CompanyEarnings *company_earnings = nullptr;
};
} // namespace avapi
#endif