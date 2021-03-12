#include "rapidcsv.h"
#include "avapi/misc.hpp"
#include "avapi/Company.hpp"
#include "avapi/Company/Stock.hpp"
#include "avapi/Company/Earnings.hpp"
#include "avapi/Company/Overview.hpp"

namespace avapi {

/// @brief Company default Constructor
Company::Company() {}

/// @brief Company symbol/api_key constructor
/// @param symbol The stock symbol e.g. "AAPL"
/// @param api_key Alpha Vantage API Key
Company::Company(const std::string &symbol, const std::string &api_key)
    : symbol(symbol)
{
    this->api_key = api_key;
}

Company::~Company()
{
    if (company_stock != nullptr)
        delete company_stock;

    if (company_overview != nullptr)
        delete company_overview;

    if (company_earnings != nullptr)
        delete company_earnings;
}

void Company::setApiKey(const std::string &key)
{
    if (company_stock != nullptr)
        company_stock->api_key = key;

    if (company_overview != nullptr)
        company_overview->api_key = key;

    if (company_earnings != nullptr)
        company_earnings->api_key = key;
}

void Company::setSymbol(const std::string &symbol)
{
    this->symbol = symbol;

    if (company_stock != nullptr)
        company_stock->symbol = symbol;

    if (company_overview != nullptr)
        company_overview->symbol = symbol;

    if (company_earnings != nullptr)
        company_earnings->symbol = symbol;
}

CompanyStock *Company::Stock()
{
    if (company_stock == nullptr) {
        company_stock = new CompanyStock(symbol, api_key);
        return company_stock;
    }
    else {
        return company_stock;
    }
}

CompanyOverview *Company::Overview(const bool &update)
{
    if (company_overview == nullptr) {
        company_overview = new CompanyOverview(symbol, api_key);
    }
    else if (update) {
        delete company_overview;
        company_overview = new CompanyOverview(symbol, api_key);
    }
    return company_overview;
}

CompanyEarnings *Company::Earnings(const bool &update)
{
    if (company_earnings == nullptr) {
        company_earnings = new CompanyEarnings(symbol, api_key);
    }
    else if (update) {
        delete company_earnings;
        company_earnings = new CompanyEarnings(symbol, api_key);
    }
    return company_earnings;
}
} // namespace avapi