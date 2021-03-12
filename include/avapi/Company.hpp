#ifndef COMPANY_H
#define COMPANY_H
#include "Company/Stock.hpp"
#include "Company/Overview.hpp"
#include "Company/Earnings.hpp"

namespace avapi {

class Company {
public:
    Company();
    explicit Company(const std::string &symbol, const std::string &key = "");
    ~Company();

    void setApiKey(const std::string &key);
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