#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include "avapi/company/Earnings.hpp"
#include "avapi/misc.hpp"

namespace avapi {

CompanyEarnings::CompanyEarnings(const std::string &symbol,
                                 const std::string &api_key)
    : symbol(symbol)
{
    api_call.api_key = api_key;
    updateEarnings();
}

CompanyEarnings::~CompanyEarnings() {}

void CompanyEarnings::updateEarnings()
{
    api_call.resetQuery();
    api_call.setFieldValue(Url::FUNCTION, "EARNINGS");
    api_call.setFieldValue(Url::SYMBOL, symbol);

    std::string data = api_call.curlQuery();

    // Parse annual earnings data
    nlohmann::json annual = nlohmann::json::parse(data)["annualEarnings"];
    for (auto &field : annual) {
        annual_earnings.push_back(
            {field["fiscalDateEnding"], field["reportedEPS"]});
    }

    // Parse quarterly earnings data
    nlohmann::json quarterly = nlohmann::json::parse(data)["quarterlyEarnings"];
    for (auto &field : quarterly) {
        quarterly_earnings.push_back(
            {field["fiscalDateEnding"], field["reportedDate"],
             field["reportedEPS"], field["estimatedEPS"], field["surprise"],
             field["surprisePercentage"]});
    }
}

void CompanyEarnings::setApiKey(const std::string &api_key)
{
    api_call.api_key = api_key;
}

} // namespace avapi