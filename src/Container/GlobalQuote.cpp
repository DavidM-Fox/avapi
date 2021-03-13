#include <iostream>
#include <fmt/core.h>
#include "rapidcsv.h"
#include "avapi/ApiCall.hpp"
#include "avapi/misc.hpp"
#include "avapi/Container/GlobalQuote.hpp"

namespace avapi {

/// @brief Default Constructor
GlobalQuote::GlobalQuote()
    : symbol(""), ApiCall(""), timestamp(0), quote_data({0})
{
}

/// @brief  GlobalQuote constructor
/// @param  symbol: The stock symbol e.g. "TSLA"
/// @param  key: An Alpha Vantage API Key
GlobalQuote::GlobalQuote(const std::string &symbol, const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    Update();
}

/// @brief  Update GlobalQuote data
void GlobalQuote::Update()
{
    resetQuery();

    // Only three parameters needed for GlobalQuote
    setFieldValue(Url::Field::FUNCTION, "GLOBAL_QUOTE");
    setFieldValue(Url::Field::SYMBOL, symbol);
    setFieldValue(Url::Field::DATA_TYPE, "csv");

    // Download csv data for global quote
    std::stringstream csv(curlQuery());

    // Get global quote row from csv std::string
    rapidcsv::Document doc(csv);
    try {

        size_t count = doc.GetRowCount();
        if (count == 0) {
            throw std::exception("avapi/CompanyStock.cpp: avapi exception: "
                                 "'CompanyStock::GlobalQuote': "
                                 "Invalid CSV response from Alpha Vantage.");
        }
    }
    catch (std::exception &ex) {
        std::cerr << "Exception Caught: " << ex.what();
    }

    std::vector<std::string> data = doc.GetRow<std::string>(0);

    // Save and convert latestDay and then erase it along with symbol
    std::time_t timestamp = avapi::toUnixTimestamp(data[6]);
    data.erase(data.begin() + 6);
    data.erase(data.begin());

    // Convert to vector of floats
    std::vector<float> data_f(data.size());
    transform(data.begin(), data.end(), data_f.begin(),
              [](std::string const &val) { return std::stof(val); });

    this->timestamp = timestamp;
    this->quote_data = data_f;
}

/// @brief  Print formatted GlobalQuote data
void GlobalQuote::printData()
{

    std::cout << std::string(29, '-') << '\n';
    fmt::print("|{:^27}|\n", symbol + " Global Quote");
    std::cout << std::string(29, '-') << '\n';

    fmt::print("|{:<12}{:>15}|\n", "Timestamp:", timestamp);

    for (size_t i = 0; i < quote_data.size(); ++i) {
        fmt::print("|{:<12}{:>15.2f}|\n", headers[i] + ":", quote_data[i]);
    }
}

const std::vector<std::string> GlobalQuote::headers{
    "Open",   "High",       "Low",    "Close",
    "Volume", "Prev_Close", "Change", "Change%"};

} // namespace avapi
