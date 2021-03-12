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

    CompanyEarnings *Earnings(const bool &update = false);
    CompanyOverview *Overview(const bool &update = false);
    CompanyStock *Stock();

private:
    std::string symbol;
    std::string api_key;

    CompanyStock *company_stock = nullptr;
    CompanyOverview *company_overview = nullptr;
    CompanyEarnings *company_earnings = nullptr;
};

} // namespace avapi
#endif