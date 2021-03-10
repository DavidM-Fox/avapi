#include "avapi/Company.hpp"
#include "avapi/company/Stock.hpp"
#include "avapi/company/Earnings.hpp"
#include "avapi/company/Overview.hpp"
#include "avapi/misc.hpp"
#include "rapidcsv.h"

namespace avapi {

/// @brief Company default Constructor
Company::Company() {}

/// @brief Company symbol/api_key constructor
/// @param symbol The stock symbol e.g. "AAPL"
/// @param api_key Alpha Vantage API Key
Company::Company(const std::string &symbol, const std::string &api_key)
    : symbol(symbol)
{

    api_call.api_key = api_key;
    api_call.output_size = "compact";

    stockConfig();
    overviewConfig();
    earningsConfig();
}

Company::~Company()
{
    if (company_overview != nullptr)
        delete company_overview;

    if (company_earnings != nullptr)
        delete company_earnings;
}

void Company::setApiKey(const std::string &api_key)
{
    api_call.api_key = api_key;
}

void Company::setSymbol(const std::string &symbol)
{
    this->symbol = symbol;

    company_stock->symbol = symbol;
    company_earnings->symbol = symbol;
    company_overview->symbol = symbol;
}

CompanyStock *Company::Stock() { return company_stock; }

CompanyOverview *Company::Overview(const bool &update)
{
    if (update) {
    }
    else {
    }
    return company_overview;
}

CompanyEarnings *Company::Earnings(const bool &update)
{
    if (update) {
    }
    else {
    }
    return company_earnings;
}

void Company::stockConfig()
{
    // Check if object has been created
    if (company_stock == nullptr)
        company_stock = new CompanyStock(symbol, api_call.api_key);
    else
        company_stock->symbol = symbol;
}

void Company::overviewConfig()
{
    // Check if object has been created
    if (company_overview == nullptr)
        company_overview = new CompanyOverview(this->symbol);
    else
        company_overview->symbol = this->symbol;
}

void Company::earningsConfig()
{
    // Check if object has been created
    if (company_earnings == nullptr)
        company_earnings = new CompanyEarnings(this->symbol);
    else
        company_earnings->symbol = this->symbol;
}
} // namespace avapi