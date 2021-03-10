#include <algorithm>
#include <sstream>
#include <nlohmann/json.hpp>
#include "rapidcsv.h"
#include "avapi/ApiCall.hpp"
#include "avapi/misc.hpp"
#include "avapi/TimeSeries.hpp"
#include "avapi/company/Stock.hpp"

namespace avapi {

/// @brief   avapi::CompanyStock default constructor
CompanyStock::CompanyStock()
{
    this->symbol = "";
    api_call.api_key = "";
    api_call.output_size = "compact";
}

/// @brief   avapi::CompanyStock constructor
/// @param   symbol The stock symbol of interest
CompanyStock::CompanyStock(const std::string &symbol)
{
    this->symbol = symbol;
    api_call.api_key = "";
    api_call.output_size = "compact";
}

/// @brief   avapi::CompanyStock constructor
/// @param   symbol The stock symbol of interest
/// @param   api_key The Alpha Vantage API key to use
CompanyStock::CompanyStock(const std::string &symbol,
                           const std::string &api_key)
{
    this->symbol = symbol;
    api_call.api_key = api_key;
    api_call.output_size = "compact";
}

/// @brief   Set the TimeSeries output size from Alpha Vantage
/// @param   size enum class SeriesSize [COMPACT, FULL]
void CompanyStock::setOutputSize(const SeriesSize &size)
{
    if (size == SeriesSize::COMPACT)
        api_call.output_size = "compact";
    else if (size == SeriesSize::FULL)
        api_call.output_size = "full";
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
    api_call.resetQuery();

    std::string title;
    std::string function = series_function[static_cast<int>(type)];

    // Check if intraday (Uses different parameters than daily, weekly, monthly)
    if (type == SeriesType::INTRADAY) {
        api_call.setFieldValue(Url::FUNCTION, function);
        api_call.setFieldValue(Url::INTERVAL, interval);
        if (adjusted) {
            api_call.setFieldValue(Url::ADJUSTED, "true");
            title = function + " (" + interval + ", Adjusted)";
        }
        else {
            api_call.setFieldValue(Url::ADJUSTED, "false");
            title = function + " (" + interval + ", Non-Adjusted)";
        }
    }
    else {
        if (adjusted) {
            api_call.setFieldValue(Url::FUNCTION, function + "_ADJUSTED");
            title = function + " (Adjusted)";
        }
        else {
            api_call.setFieldValue(Url::FUNCTION, function);
            title = function + " (Non-Adjusted)";
        }
    }

    // Set other needed API fields
    api_call.setFieldValue(Url::SYMBOL, symbol);
    api_call.setFieldValue(Url::OUTPUT_SIZE, api_call.output_size);
    api_call.setFieldValue(Url::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(api_call.curlQuery());
    series.symbol = symbol;
    series.type = type;
    series.is_adjusted = adjusted;
    series.title = symbol + ": " + title;
    return series;
}

/// @brief GlobalQuote constructor
/// @param timestamp A Unix timestamp
/// @param data std::vector<float> ordered: [Open, High, Low, Close, Volume,
/// Prev_Close, Change, Change%]
CompanyStock::GlobalQuote::GlobalQuote(const std::string &symbol,
                                       const std::time_t &timestamp,
                                       const std::vector<float> &data)
    : symbol(symbol), timestamp(timestamp), quote_data(data)
{
}

/// @brief print formatted avapi::GlobalQuote data
void CompanyStock::GlobalQuote::printData()
{
    std::cout << std::setw(12) << std::left << "Timestamp:";
    std::cout << std::setw(15) << std::right << timestamp << '\n';
    for (size_t i = 0; i < quote_data.size(); ++i) {
        std::string header = headers[i] + ":";
        std::cout << std::setw(12) << std::left << header;
        std::cout << std::setw(15) << std::right << std::fixed
                  << std::setprecision(2) << quote_data[i];
        std::cout << '\n';
    }
}

const std::vector<std::string> CompanyStock::GlobalQuote::headers{
    "Open",   "High",       "Low",    "Close",
    "Volume", "Prev_Close", "Change", "Change%"};

/// @brief   Return the symbol's latest global quote
/// @returns The symbol's global quote as an avapi::GlobalQuote object
CompanyStock::GlobalQuote CompanyStock::getGlobalQuote()
{
    api_call.resetQuery();

    // Only three parameters needed for GlobalQuote
    api_call.setFieldValue(Url::FUNCTION, "GLOBAL_QUOTE");
    api_call.setFieldValue(Url::SYMBOL, symbol);
    api_call.setFieldValue(Url::DATA_TYPE, "csv");

    // Download csv data for global quote
    std::stringstream csv(api_call.curlQuery());

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
        std::cerr << " Returning empty Global Quote.\n";
        return {"NULL", 0, std::vector<float>(8, 0.0)};
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

    return {symbol, timestamp, data_f};
}

void CompanyStock::setApiKey(const std::string &api_key)
{
    api_call.api_key = api_key;
}

const std::vector<std::string> CompanyStock::series_function = {
    "TIME_SERIES_INTRADAY", "TIME_SERIES_DAILY", "TIME_SERIES_WEEKLY",
    "TIME_SERIES_MONTHLY"};
} // namespace avapi