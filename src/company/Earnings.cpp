#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "avapi/Company/Earnings.hpp"
#include "avapi/misc.hpp"

namespace avapi {

/// @brief Default constructor
CompanyEarnings::CompanyEarnings() : symbol(""), ApiCall("") {}

/// @brief Constructor
/// @param symbol The company symbol e.g. "TSLA"
/// @param key Alpha Vantage API key
CompanyEarnings::CompanyEarnings(const std::string &symbol,
                                 const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    Update();
}

/// @brief Get the current annual and quarterly earnings
void CompanyEarnings::Update()
{
    if (symbol == "" || api_key == "") {
        std::cerr << "avapi/Company/Earnings.cpp: Warning: "
                     "'CompanyEarnings::Update': symbol or api_key is empty. "
                     "No values were updated.\n";
        return;
    }

    resetQuery();
    setFieldValue(Url::FUNCTION, "EARNINGS");
    setFieldValue(Url::SYMBOL, symbol);

    std::string data = curlQuery();

    // Parse annual earnings data
    nlohmann::json annual = nlohmann::json::parse(data)["annualEarnings"];
    annual_earnings.data.clear();
    for (auto &field : annual) {
        annual_earnings.data.push_back(
            {field["fiscalDateEnding"], field["reportedEPS"]});
    }

    // Parse quarterly earnings data
    nlohmann::json quarterly = nlohmann::json::parse(data)["quarterlyEarnings"];
    quarterly_earnings.data.clear();
    for (auto &field : quarterly) {
        quarterly_earnings.data.push_back(
            {field["fiscalDateEnding"], field["reportedDate"],
             field["reportedEPS"], field["estimatedEPS"], field["surprise"],
             field["surprisePercentage"]});
    }
}

} // namespace avapi