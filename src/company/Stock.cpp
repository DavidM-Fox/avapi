#include <algorithm>
#include <sstream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "rapidcsv.h"
#include "avapi/ApiCall.hpp"
#include "avapi/misc.hpp"
#include "avapi/Container/TimeSeries.hpp"
#include "avapi/Company/Stock.hpp"

namespace avapi {

/// @brief   avapi::CompanyStock default constructor
CompanyStock::CompanyStock() : symbol(symbol), ApiCall("")
{
    output_size = "compact";
}

/// @brief   avapi::CompanyStock constructor
/// @param   symbol The stock symbol of interest
CompanyStock::CompanyStock(const std::string &symbol)
    : symbol(symbol), ApiCall("")
{
    output_size = "compact";
}

/// @brief   avapi::CompanyStock constructor
/// @param   symbol The stock symbol of interest
/// @param   api_key The Alpha Vantage API key to use
CompanyStock::CompanyStock(const std::string &symbol, const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    output_size = "compact";
}

/// @brief   Set the TimeSeries output size from Alpha Vantage
/// @param   size enum class SeriesSize [COMPACT, FULL]
void CompanyStock::setOutputSize(const SeriesSize &size)
{
    if (size == SeriesSize::COMPACT)
        output_size = "compact";
    else if (size == SeriesSize::FULL)
        output_size = "full";
}

/// @brief   Get an avapi::TimeSeries for a stock symbol of interest.
/// @param   type enum class avapi::SeriesType
/// @param   adjusted Whether or not the data should have adjusted values
/// @param   interval The interval for avapi::series::INTRADAY, ignored
/// otherwise (default = "30min")
/// @returns An avapi::TimeSeries: [open,high,low,close,volume]
/// or an adjusted avapi::TimeSeries:
/// [open,high,low,close,adjusted_close,volume,dividend_amount,split_coefficient]
TimeSeries CompanyStock::getTimeSeries(const avapi::SeriesType &type,
                                       const bool &adjusted,
                                       const std::string &interval)
{
    resetQuery();

    std::string title;
    std::string function = series_function[static_cast<int>(type)];

    // Check if intraday (Uses different parameters than daily, weekly, monthly)
    if (type == SeriesType::INTRADAY) {
        setFieldValue(Url::FUNCTION, function);
        setFieldValue(Url::INTERVAL, interval);
        if (adjusted) {
            setFieldValue(Url::ADJUSTED, "true");
            title = function + " (" + interval + ", Adjusted)";
        }
        else {
            setFieldValue(Url::ADJUSTED, "false");
            title = function + " (" + interval + ", Non-Adjusted)";
        }
    }
    else {
        if (adjusted) {
            setFieldValue(Url::FUNCTION, function + "_ADJUSTED");
            title = function + " (Adjusted)";
        }
        else {
            setFieldValue(Url::FUNCTION, function);
            title = function + " (Non-Adjusted)";
        }
    }

    // Set other needed API fields
    setFieldValue(Url::SYMBOL, symbol);
    setFieldValue(Url::OUTPUT_SIZE, output_size);
    setFieldValue(Url::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(curlQuery());
    series.symbol = symbol;
    series.type = type;
    series.is_adjusted = adjusted;
    series.title = symbol + ": " + title;
    return series;
}

GlobalQuote CompanyStock::getGlobalQuote() { return {symbol, api_key}; }

const std::vector<std::string> CompanyStock::series_function = {
    "TIME_SERIES_INTRADAY", "TIME_SERIES_DAILY", "TIME_SERIES_WEEKLY",
    "TIME_SERIES_MONTHLY"};

} // namespace avapi