#ifndef COMPANY_H
#define COMPANY_H
#include "avapi/Company/Stock.hpp"
#include "avapi/Company/Overview.hpp"
#include "avapi/Company/Earnings.hpp"

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
    std::string &Symbol() { return symbol; }

    CompanyStock *Stock();
    CompanyOverview *Overview(const bool &update = false);
    CompanyEarnings *Earnings(const bool &update = false);

private:
    std::string symbol;
    std::string api_key;

    CompanyStock *company_stock = nullptr;
    CompanyOverview *company_overview = nullptr;
    CompanyEarnings *company_earnings = nullptr;
};

} // namespace avapi
#endif