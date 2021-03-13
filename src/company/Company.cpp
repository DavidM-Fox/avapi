#include "avapi/Company/Company.hpp"
#include "avapi/Company/Earnings.hpp"
#include "avapi/Company/Overview.hpp"
#include "avapi/Company/Stock.hpp"

namespace avapi {

/// @brief Default Constructor
Company::Company()
    : symbol(""), api_key(""), company_earnings(nullptr),
      company_overview(nullptr), company_stock(nullptr)
{
}

/// @brief Constructor
/// @param symbol: The stock symbol e.g. "AAPL"
/// @param key: Alpha Vantage API Key
Company::Company(const std::string &symbol, const std::string &key)
    : symbol(symbol), api_key(key), company_earnings(nullptr),
      company_overview(nullptr), company_stock(nullptr)
{
}

/// @brief Company deconstructor, deletes members
Company::~Company()
{
    if (company_stock != nullptr) {
        delete company_stock;
    }
    if (company_overview != nullptr) {
        delete company_overview;
    }
    if (company_earnings != nullptr) {
        delete company_earnings;
    }
}

/// @brief Set the Alpha Vantage API key for Company and its components
/// @param key - Alpha Vantage API key
void Company::setApiKey(const std::string &key)
{
    this->api_key = key;
    if (company_stock != nullptr) {
        company_stock->api_key = key;
    }
    if (company_overview != nullptr) {
        company_overview->api_key = key;
    }
    if (company_earnings != nullptr) {
        company_earnings->api_key = key;
    }
}

/// @brief Set the Company symbol
/// @param key - Alpha Vantage API key
void Company::setSymbol(const std::string &symbol)
{
    this->symbol = symbol;

    if (company_stock != nullptr) {
        company_stock->symbol = symbol;
    }
    if (company_overview != nullptr) {
        company_overview->symbol = symbol;
    }
    if (company_earnings != nullptr) {
        company_earnings->symbol = symbol;
    }
}

/// @brief Return an avapi::CompanyStock* for this Company, the first time this
/// is called will create a new avapi::CompanyStock object
CompanyStock *Company::Stock()
{
    if (company_stock == nullptr) {
        company_stock = new CompanyStock(symbol, api_key);
    }
    return company_stock;
}

/// @brief Return an avapi::CompanyOverview* for this Company, the first time
/// this is called will create a new avapi::CompanyOverview object
CompanyOverview *Company::Overview()
{
    if (company_overview == nullptr) {
        company_overview = new CompanyOverview(symbol, api_key);
    }
    return company_overview;
}

/// @brief Return an avapi::CompanyEarnings* for this Company, the first time
/// this is called will create a new avapi::CompanyEarnings object
CompanyEarnings *Company::Earnings()
{
    if (company_earnings == nullptr) {
        company_earnings = new CompanyEarnings(symbol, api_key);
    }
    return company_earnings;
}
} // namespace avapi