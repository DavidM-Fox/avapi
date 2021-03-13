#ifndef COMPANY_H
#define COMPANY_H
#include "avapi/Company/Earnings.hpp"
#include "avapi/Company/Overview.hpp"
#include "avapi/Company/Stock.hpp"

namespace avapi {

class Company {
public:
    Company();
    explicit Company(const std::string &symbol, const std::string &key = "");
    ~Company();

    void setApiKey(const std::string &key);
    void setSymbol(const std::string &symbol);
    std::string &Symbol() { return symbol; }

    CompanyEarnings *Earnings();
    CompanyOverview *Overview();
    CompanyStock *Stock();

private:
    std::string symbol;
    std::string api_key;

    CompanyStock *company_stock;
    CompanyOverview *company_overview;
    CompanyEarnings *company_earnings;
};

} // namespace avapi
#endif