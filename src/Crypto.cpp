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
    m_cryptoApiCall.api_key = "";
    m_cryptoApiCall.output_size = "compact";
}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
Crypto::Crypto(const std::string &symbol)
{
    this->m_symbol = symbol;
    m_cryptoApiCall.api_key = "";
    m_cryptoApiCall.output_size = "compact";
}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
/// @param   api_key The Alpha Vantage API key to use
Crypto::Crypto(const std::string &symbol, const std::string &api_key)
{
    this->m_symbol = symbol;
    m_cryptoApiCall.api_key = api_key;
    m_cryptoApiCall.output_size = "compact";
}

/// @brief   Set the TimeSeries output size from Alpha Vantage
/// @param   size enum class SeriesSize [COMPACT, FULL]
void Crypto::setOutputSize(const SeriesSize &size)
{
    if (size == SeriesSize::COMPACT)
        m_cryptoApiCall.output_size = "compact";
    else if (size == SeriesSize::FULL)
        m_cryptoApiCall.output_size = "full";
}

/// @brief   Get an avapi::TimeSeries for a crypto symbol of interest.
/// @param   type enum class avapi::SeriesType
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::TimeSeries ordered [open,high,low,close,volume]
TimeSeries Crypto::getTimeSeries(const SeriesType &type,
                                 const std::string &market)
{
    m_cryptoApiCall.resetQuery();

    std::string function;

    SeriesType check = type;

    // Intraday not available from Alpha Vantage
    if (check == SeriesType::INTRADAY) {
        check = SeriesType::DAILY;
        std::cerr << "avapi/Crypto.cpp: exception: "
                     "'avapi::Crypto::getTimeSeries': Intraday not available "
                     "from Alpha Vantage, returning a daily TimeSeries.\n";
    }

    function = m_seriesFunctionStrings[static_cast<int>(check)];
    m_cryptoApiCall.setFieldValue(Url::FUNCTION, function);

    // Set other needed API fields
    m_cryptoApiCall.setFieldValue(Url::SYMBOL, m_symbol);
    m_cryptoApiCall.setFieldValue(Url::MARKET, market);
    m_cryptoApiCall.setFieldValue(Url::OUTPUT_SIZE,
                                  m_cryptoApiCall.output_size);
    m_cryptoApiCall.setFieldValue(Url::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(m_cryptoApiCall.curlQuery(), true);
    series.m_symbol = m_symbol;
    series.m_type = check;
    series.m_adjusted = false;
    series.m_market = market;
    series.m_title = m_symbol + ": " + function;
    return series;
}

/// @brief   Get the current exchange rate for a specific market
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::ExchangeRate object: [Exchange, Bid, Ask]
ExchangeRate Crypto::getExchangeRate(const std::string &market)
{
    m_cryptoApiCall.resetQuery();

    m_cryptoApiCall.setFieldValue(Url::FUNCTION, "CURRENCY_EXCHANGE_RATE");
    m_cryptoApiCall.setFieldValue(Url::FROM_CURRENCY, m_symbol);
    m_cryptoApiCall.setFieldValue(Url::TO_CURRENCY, market);

    std::string data = m_cryptoApiCall.curlQuery();

    nlohmann::json json =
        nlohmann::json::parse(data)["Realtime Currency Exchange Rate"];

    std::time_t timestamp = toUnixTimestamp(json["6. Last Refreshed"]);

    std::vector<float> exchange_data = {
        std::stof(std::string(json["5. Exchange Rate"])),
        std::stof(std::string(json["8. Bid Price"])),
        std::stof(std::string(json["9. Ask Price"]))};

    return {m_symbol, market, timestamp, exchange_data};
}
const std::vector<std::string> Crypto::m_seriesFunctionStrings = {
    "DIGITAL_CURRENCY_DAILY", "DIGITAL_CURRENCY_WEEKLY",
    "DIGITAL_CURRENCY_MONTHLY"};

} // namespace avapi