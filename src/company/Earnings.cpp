#include <iostream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "avapi/Company/Earnings.hpp"
#include "avapi/misc.hpp"

namespace avapi {

CompanyEarnings::CompanyEarnings(const std::string &symbol,
                                 const std::string &key)
    : symbol(symbol), ApiCall(key), annual_earnings(new AnnualEarnings(symbol)),
      quarterly_earnings(new QuarterlyEarnings(symbol))
{
    Update();
}

CompanyEarnings::~CompanyEarnings()
{
    delete annual_earnings;
    delete quarterly_earnings;
}

void CompanyEarnings::Update()
{
    resetQuery();
    setFieldValue(Url::FUNCTION, "EARNINGS");
    setFieldValue(Url::SYMBOL, symbol);

    std::string data = curlQuery();

    // Parse annual earnings data
    nlohmann::json annual = nlohmann::json::parse(data)["annualEarnings"];
    for (auto &field : annual) {
        annual_earnings->data.push_back(
            {field["fiscalDateEnding"], field["reportedEPS"]});
    }

    // Parse quarterly earnings data
    nlohmann::json quarterly = nlohmann::json::parse(data)["quarterlyEarnings"];
    for (auto &field : quarterly) {
        quarterly_earnings->data.push_back(
            {field["fiscalDateEnding"], field["reportedDate"],
             field["reportedEPS"], field["estimatedEPS"], field["surprise"],
             field["surprisePercentage"]});
    }
}

void CompanyEarnings::AnnualEarnings::printData()
{
    std::cout << std::string(38, '-') << '\n';
    fmt::print("|{:^36}|\n", symbol + " Annual Earnings");
    fmt::print("|{:^20}|{:^15}|\n", "Fiscal Date Ending", "Reported EPS");
    std::cout << std::string(38, '-') << '\n';

    for (auto &report : data) {
        fmt::print("|{:^20}|{:>15.2f}|\n", report.fiscal_date_ending,
                   std::stof(report.reported_eps));
    }
}

void CompanyEarnings::QuarterlyEarnings::printData()
{
    std::cout << std::string(102, '-') << '\n';
    fmt::print("|{:^100}|\n", symbol + " Quarterly Earnings");
    fmt::print("|{:^20}|{:^15}|{:^15}|{:^15}|{:^15}|{:^15}|\n",
               "Fiscal Date Ending", "Reported Date", "Reported EPS",
               "Estimated EPS", "Surprise", "Surprise %");
    std::cout << std::string(102, '-') << '\n';

    for (auto &report : data) {
        if (report.estimated_eps == "None") {
            fmt::print("|{:^20}|{:^15}|{:>15.2f}|{:>15}|{:>15}|{:>15}|\n",
                       report.fiscal_date_ending, report.reported_date,
                       std::stof(report.reported_eps), "None", "None", "None");
        }
        else {
            fmt::print(
                "|{:^20}|{:^15}|{:>15.2f}|{:>15.2f}|{:>15.2f}|{:>15.2f}|\n",
                report.fiscal_date_ending, report.reported_date,
                std::stof(report.reported_eps), std::stof(report.estimated_eps),
                std::stof(report.surprise),
                std::stof(report.surprise_percentage));
        }
    }
}

} // namespace avapi