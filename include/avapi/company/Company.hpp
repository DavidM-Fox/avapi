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

    std::unique_ptr<CompanyEarnings> &earnings();
    std::unique_ptr<CompanyOverview> &overview();
    std::unique_ptr<CompanyStock> &stock();

private:
    std::string symbol;
    std::string api_key;

    std::unique_ptr<CompanyEarnings> company_earnings;
    std::unique_ptr<CompanyOverview> company_overview;
    std::unique_ptr<CompanyStock> company_stock;
};

} // namespace avapi
#endif