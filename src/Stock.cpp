#include <algorithm>
#include <sstream>
#include "rapidcsv.h"
#include "avapi/ApiCall.hpp"
#include "avapi/TimeSeries.hpp"
#include "avapi/GlobalQuote.hpp"
#include "avapi/Stock.hpp"

namespace avapi {

/// @brief   avapi::Stock constructor
/// @param   symbol The stock symbol of interest
/// @param   api_key The Alpha Vantage API key to use
Stock::Stock(const std::string &symbol, const std::string &api_key)
    : m_symbol(symbol), m_apiKey(api_key)
{

    // Force symbol to be capitalized
    std::transform(m_symbol.begin(), m_symbol.end(), m_symbol.begin(),
                   ::toupper);
    setOutputSize("compact");
}

/// @brief   Get an avapi::TimeSeries for a stock symbol of interest.
/// @param   type enum avapi::series::type for TimeSeries type
/// @param   adjusted Whether or not the data should have adjusted values
/// @param   interval The interval for avapi::series::INTRADAY, ignored
/// otherwise (default = "30min")
/// @returns An avapi::TimeSeries: [open,high,low,close,volume]
/// or an adjusted avapi::TimeSeries:
/// [open,high,low,close,adjusted_close,volume,dividend_amount,split_coefficient]
TimeSeries Stock::getTimeSeries(const TimeSeries::type &type,
                                const bool &adjusted,
                                const std::string &interval)
{
    // Create new ApiCall object for this method
    m_apiCall = new ApiCall(m_apiKey);

    std::string title;

    // Set API function field
    switch (type) {
    case TimeSeries::type::INTRADAY:
        m_apiCall->setFieldValue(Url::FUNCTION, "TIME_SERIES_INTRADAY");
        m_apiCall->setFieldValue(Url::INTERVAL, interval);
        if (adjusted) {
            m_apiCall->setFieldValue(Url::ADJUSTED, "true");
            title =
                "Intraday Time Series (Adjusted, interval = " + interval + ')';
        }
        else {
            m_apiCall->setFieldValue(Url::ADJUSTED, "false");
            title =
                "Intraday Time Series (Non-Adjusted, interval = " + interval +
                ')';
        }
        break;

    case TimeSeries::type::DAILY:
        if (adjusted) {
            m_apiCall->setFieldValue(Url::FUNCTION,
                                     "TIME_SERIES_DAILY_ADJUSTED");
            title = "Daily Time Series (Adjusted)";
        }

        else {
            m_apiCall->setFieldValue(Url::FUNCTION, "TIME_SERIES_DAILY");
            title = "Daily Time Series (Non-Adjusted)";
        }
        break;

    case TimeSeries::type::WEEKLY:
        if (adjusted) {
            m_apiCall->setFieldValue(Url::FUNCTION,
                                     "TIME_SERIES_WEEKLY_ADJUSTED");
            title = "Weekly Time Series (Adjusted)";
        }
        else {
            m_apiCall->setFieldValue(Url::FUNCTION, "TIME_SERIES_WEEKLY");
            title = "Weekly Time Series (Non-Adjusted)";
        }
        break;

    case TimeSeries::type::MONTHLY:
        if (adjusted) {
            m_apiCall->setFieldValue(Url::FUNCTION,
                                     "TIME_SERIES_WEEKLY_ADJUSTED");
            title = "Monthly Time Series (Adjusted)";
        }
        else {
            m_apiCall->setFieldValue(Url::FUNCTION, "TIME_SERIES_WEEKLY");
            title = "Monthly Time Series (Non-Adjusted)";
        }
        break;

    default:
        std::invalid_argument ex("Incorrect avapi::Stock::function argument");
        throw ex;
    }

    // Set other needed API fields
    m_apiCall->setFieldValue(Url::SYMBOL, m_symbol);
    m_apiCall->setFieldValue(Url::OUTPUT_SIZE, m_outputSize);
    m_apiCall->setFieldValue(Url::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(m_apiCall->Curl());
    delete m_apiCall;

    series.setSymbol(m_symbol);
    series.setType(type);
    series.setAdjusted(adjusted);
    series.setTitle(m_symbol + ": " + title);
    return series;
}

/// @brief Set the output size for Alpha Vantage API calls
/// @param size The output size: "compact" or "full" (default = "compact")
void Stock::setOutputSize(const std::string &size) { m_outputSize = size; }

/// @brief   Return the symbol's latest global quote
/// @returns The symbol's global quote as an avapi::GlobalQuote objecta
GlobalQuote Stock::getGlobalQuote()
{
    // Create new ApiCall object for this method
    m_apiCall = new ApiCall(m_apiKey);

    m_apiCall->setFieldValue(Url::FUNCTION, "GLOBAL_QUOTE");
    m_apiCall->setFieldValue(Url::SYMBOL, m_symbol);
    m_apiCall->setFieldValue(Url::DATA_TYPE, "csv");

    // Download csv data for global quote
    std::stringstream csv(m_apiCall->Curl());
    delete m_apiCall;

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
} // namespace avapi