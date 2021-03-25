#include <sstream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "avapi/ApiCall.hpp"
#include "avapi/misc.hpp"
#include "avapi/Container/TimeSeries.hpp"
#include "avapi/Company/Stock.hpp"
#include <rapidcsv.h>

namespace avapi {

/// @brief Default Constructor
CompanyStock::CompanyStock() : symbol(""), ApiCall("")
{
    output_size = "compact";
}

/// @brief Constructor
/// @param symbol: The stock symbol of interest
/// @param api_key: The Alpha Vantage API key to use
CompanyStock::CompanyStock(const std::string &symbol, const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    output_size = "compact";
}

/// @brief   Get an avapi::TimeSeries for a stock symbol of interest.
/// @param   type: The avapi::SeriesType
/// @param   adjusted: Adjusted or Non-Adjusted data
/// @param   interval: The interval for INTRADAY, ignored otherwise (default =
/// "30min")
TimeSeries CompanyStock::getTimeSeries(const avapi::SeriesType &type,
                                       const bool &adjusted,
                                       const std::string &interval)
{
    resetQuery();

    std::string title;
    std::string function = series_function[static_cast<int>(type)];

    // Check if intraday (Uses different parameters than daily, weekly, monthly)
    if (type == SeriesType::INTRADAY) {
        setFieldValue(Url::Field::FUNCTION, function);
        setFieldValue(Url::Field::INTERVAL, interval);
        if (adjusted) {
            setFieldValue(Url::Field::ADJUSTED, "true");
            title = function + " (" + interval + ", Adjusted)";
        }
        else {
            setFieldValue(Url::Field::ADJUSTED, "false");
            title = function + " (" + interval + ", Non-Adjusted)";
        }
    }
    else {
        if (adjusted) {
            setFieldValue(Url::Field::FUNCTION, function + "_ADJUSTED");
            title = function + " (Adjusted)";
        }
        else {
            setFieldValue(Url::Field::FUNCTION, function);
            title = function + " (Non-Adjusted)";
        }
    }

    // Set other needed API fields
    setFieldValue(Url::Field::SYMBOL, symbol);
    setFieldValue(Url::Field::OUTPUT_SIZE, output_size);
    setFieldValue(Url::Field::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(curlQuery());
    series.symbol = symbol;
    series.type = type;
    series.is_adjusted = adjusted;
    series.title = symbol + ": " + title;
    return series;
}

GlobalQuote CompanyStock::getGlobalQuote()
{
    // Only three parameters needed for GlobalQuote
    resetQuery();
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
    std::vector<double> data_f(data.size());
    transform(data.begin(), data.end(), data_f.begin(),
              [](std::string const &val) { return std::stod(val); });

    return {symbol, timestamp, data_f};
}

const std::vector<std::string> CompanyStock::series_function = {
    "TIME_SERIES_INTRADAY", "TIME_SERIES_DAILY", "TIME_SERIES_WEEKLY",
    "TIME_SERIES_MONTHLY"};

} // namespace avapi