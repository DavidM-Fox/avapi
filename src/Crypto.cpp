#include <algorithm>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "rapidcsv.h"
#include "avapi/ApiCall.hpp"
#include "avapi/TimeSeries.hpp"
#include "avapi/ExchangeRate.hpp"
#include "avapi/Crypto.hpp"

namespace avapi {

/// @brief   avapi::Crypto Default Constructor
Crypto::Crypto()
{
    this->m_symbol = "";
    m_apiCall.m_apiKey = "";
    m_apiCall.m_outputSize = "compact";
}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
Crypto::Crypto(const std::string &symbol)
{
    this->m_symbol = symbol;
    m_apiCall.m_apiKey = "";
    m_apiCall.m_outputSize = "compact";
}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
/// @param   api_key The Alpha Vantage API key to use
Crypto::Crypto(const std::string &symbol, const std::string &api_key)
{
    this->m_symbol = symbol;
    m_apiCall.m_apiKey = api_key;
    m_apiCall.m_outputSize = "compact";
}

/// @brief   Get an avapi::TimeSeries for a crypto symbol of interest.
/// @param   type enum class avapi::SeriesType
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::TimeSeries ordered [open,high,low,close,volume]
TimeSeries Crypto::getTimeSeries(const SeriesType &type,
                                 const std::string &market)
{
    m_apiCall.ResetUrl();

    std::string title;
    std::string function;

    // Intraday not available from Alpha Vantage
    if (type == SeriesType::INTRADAY) {
        std::cerr << "\"avapi/Crypto.cpp\": Intraday not available from Alpha "
                     "Vantage, returning a daily TimeSeries.\n";
        function = "DIGITAL_CURRENCY_DAILY";
        m_apiCall.setFieldValue(Url::FUNCTION, function);
        title = function;
    }
    else {
        function = m_seriesFunctionStrings[static_cast<int>(type)];
        m_apiCall.setFieldValue(Url::FUNCTION, function);
        title = function;
    }

    // Set other needed API fields
    m_apiCall.setFieldValue(Url::SYMBOL, m_symbol);
    m_apiCall.setFieldValue(Url::MARKET, market);
    m_apiCall.setFieldValue(Url::OUTPUT_SIZE, m_apiCall.m_outputSize);
    m_apiCall.setFieldValue(Url::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(m_apiCall.Curl(), true);

    series.setSymbol(m_symbol);
    return series;
}

/// @brief   Get the current exchange rate for a specific market
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::ExchangeRate object: [Exchange, Bid, Ask]
ExchangeRate Crypto::getExchangeRate(const std::string &market)
{
    m_apiCall.ResetUrl();

    m_apiCall.setFieldValue(Url::FUNCTION, "CURRENCY_EXCHANGE_RATE");
    m_apiCall.setFieldValue(Url::FROM_CURRENCY, m_symbol);
    m_apiCall.setFieldValue(Url::TO_CURRENCY, market);

    std::string data = m_apiCall.Curl();

    nlohmann::json json =
        nlohmann::json::parse(data)["Realtime Currency Exchange Rate"];

    std::time_t timestamp = toUnixTimestamp(json["6. Last Refreshed"]);

    std::string exchange_rate = json["5. Exchange Rate"];
    std::string bid_price = json["8. Bid Price"];
    std::string ask_price = json["9. Ask Price"];

    std::vector<float> exchange_data;

    exchange_data.push_back(std::stof(exchange_rate));
    exchange_data.push_back(std::stof(bid_price));
    exchange_data.push_back(std::stof(ask_price));

    ExchangeRate exchange(m_symbol, market, timestamp, exchange_data);
    return exchange;
}
const std::vector<std::string> Crypto::m_seriesFunctionStrings = {
    "DIGITAL_CURRENCY_DAILY", "DIGITAL_CURRENCY_WEEKLY",
    "DIGITAL_CURRENCY_MONTHLY"};

} // namespace avapi