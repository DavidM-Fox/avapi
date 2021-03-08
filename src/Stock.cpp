#include <algorithm>
#include <sstream>
#include "rapidcsv.h"
#include "avapi/ApiCall.hpp"
#include "avapi/Stock.hpp"

namespace avapi {

/// @brief   avapi::Stock default constructor
Stock::Stock()
{
    this->m_symbol = "";
    m_apiCall.m_apiKey = "";
    m_apiCall.m_outputSize = "compact";
}

/// @brief   avapi::Stock constructor
/// @param   symbol The stock symbol of interest
Stock::Stock(const std::string &symbol)
{
    this->m_symbol = symbol;
    m_apiCall.m_apiKey = "";
    m_apiCall.m_outputSize = "compact";
}

/// @brief   avapi::Stock constructor
/// @param   symbol The stock symbol of interest
/// @param   api_key The Alpha Vantage API key to use
Stock::Stock(const std::string &symbol, const std::string &api_key)
{
    this->m_symbol = symbol;
    m_apiCall.m_apiKey = api_key;
    m_apiCall.m_outputSize = "compact";
}

/// @brief   Get an avapi::TimeSeries for a stock symbol of interest.
/// @param   type enum class avapi::SeriesType
/// @param   adjusted Whether or not the data should have adjusted values
/// @param   interval The interval for avapi::series::INTRADAY, ignored
/// otherwise (default = "30min")
/// @returns An avapi::TimeSeries: [open,high,low,close,volume]
/// or an adjusted avapi::TimeSeries:
/// [open,high,low,close,adjusted_close,volume,dividend_amount,split_coefficient]
TimeSeries Stock::getTimeSeries(const avapi::SeriesType &type,
                                const bool &adjusted,
                                const std::string &interval)
{
    m_apiCall.ResetUrl();

    std::string title;
    std::string function;

    // Check if intraday (Uses different parameters than daily, weekly, monthly)
    if (type == SeriesType::INTRADAY) {
        function = m_seriesFunctionStrings[static_cast<int>(type)];
        m_apiCall.setFieldValue(Url::FUNCTION, function);
        m_apiCall.setFieldValue(Url::INTERVAL, interval);
        if (adjusted) {
            m_apiCall.setFieldValue(Url::ADJUSTED, "true");
            title = function + " (" + interval + ", Adjusted)";
        }
        else {
            m_apiCall.setFieldValue(Url::ADJUSTED, "false");
            title = function + " (" + interval + ", Non-Adjusted)";
        }
    }
    else {
        function = m_seriesFunctionStrings[static_cast<int>(type)];
        if (adjusted) {
            m_apiCall.setFieldValue(Url::FUNCTION, function + "_ADJUSTED");
            title = function + " (Adjusted)";
        }
        else {
            m_apiCall.setFieldValue(Url::FUNCTION, function);
            title = function + " (Non-Adjusted)";
        }
    }

    // Set other needed API fields
    m_apiCall.setFieldValue(Url::SYMBOL, m_symbol);
    m_apiCall.setFieldValue(Url::OUTPUT_SIZE, m_apiCall.m_outputSize);
    m_apiCall.setFieldValue(Url::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(m_apiCall.Curl());
    series.setSymbol(m_symbol);
    series.setType(type);
    series.setAdjusted(adjusted);
    series.setTitle(m_symbol + ": " + title);
    return series;
}

/// @brief   Return the symbol's latest global quote
/// @returns The symbol's global quote as an avapi::GlobalQuote object
GlobalQuote Stock::getGlobalQuote()
{
    // Create new ApiCall object for this method
    m_apiCall.ResetUrl();

    // Only three parameters needed for GlobalQuote
    m_apiCall.setFieldValue(Url::FUNCTION, "GLOBAL_QUOTE");
    m_apiCall.setFieldValue(Url::SYMBOL, m_symbol);
    m_apiCall.setFieldValue(Url::DATA_TYPE, "csv");

    // Download csv data for global quote
    std::stringstream csv(m_apiCall.Curl());

    // Get global quote row from csv std::string
    rapidcsv::Document doc(csv);
    std::vector<std::string> data = doc.GetRow<std::string>(0);

    // Save and convert latestDay and then erase it along with symbol
    std::time_t timestamp = toUnixTimestamp(data[6]);
    data.erase(data.begin() + 6);
    data.erase(data.begin());

    // Convert to vector of floats
    std::vector<float> data_f(data.size());
    transform(data.begin(), data.end(), data_f.begin(),
              [](std::string const &val) { return std::stof(val); });

    GlobalQuote global_quote(m_symbol, timestamp, data_f);
    return global_quote;
}

const std::vector<std::string> Stock::m_seriesFunctionStrings = {
    "TIME_SERIES_INTRADAY", "TIME_SERIES_DAILY", "TIME_SERIES_WEEKLY",
    "TIME_SERIES_MONTHLY"};
} // namespace avapi