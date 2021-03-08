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
    this->symbol = "";
    api_call.api_key = "";
    api_call.output_size = "compact";
}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
Crypto::Crypto(const std::string &symbol)
{
    this->symbol = symbol;
    api_call.api_key = "";
    api_call.output_size = "compact";
}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
/// @param   api_key The Alpha Vantage API key to use
Crypto::Crypto(const std::string &symbol, const std::string &api_key)
{
    this->symbol = symbol;
    api_call.api_key = api_key;
    api_call.output_size = "compact";
}

/// @brief   Set the TimeSeries output size from Alpha Vantage
/// @param   size enum class SeriesSize [COMPACT, FULL]
void Crypto::setOutputSize(const SeriesSize &size)
{
    if (size == SeriesSize::COMPACT)
        api_call.output_size = "compact";
    else if (size == SeriesSize::FULL)
        api_call.output_size = "full";
}

/// @brief   Get an avapi::TimeSeries for a crypto symbol of interest.
/// @param   type enum class avapi::SeriesType
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::TimeSeries ordered [open,high,low,close,volume]
TimeSeries Crypto::getTimeSeries(const SeriesType &type,
                                 const std::string &market)
{
    api_call.resetQuery();

    std::string function;

    SeriesType check = type;

    // Intraday not available from Alpha Vantage
    if (check == SeriesType::INTRADAY) {
        check = SeriesType::DAILY;
        std::cerr << "avapi/Crypto.cpp: exception: "
                     "'avapi::Crypto::getTimeSeries': Intraday not available "
                     "from Alpha Vantage, returning a daily TimeSeries.\n";
    }

    function = series_function[static_cast<int>(check)];
    api_call.setFieldValue(Url::FUNCTION, function);

    // Set other needed API fields
    api_call.setFieldValue(Url::SYMBOL, symbol);
    api_call.setFieldValue(Url::MARKET, market);
    api_call.setFieldValue(Url::OUTPUT_SIZE, api_call.output_size);
    api_call.setFieldValue(Url::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(api_call.curlQuery(), true);
    series.symbol = symbol;
    series.type = check;
    series.is_adjusted = false;
    series.market = market;
    series.title = symbol + ": " + function;
    return series;
}

/// @brief   Get the current exchange rate for a specific market
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::ExchangeRate object: [Exchange, Bid, Ask]
ExchangeRate Crypto::getExchangeRate(const std::string &market)
{
    api_call.resetQuery();

    api_call.setFieldValue(Url::FUNCTION, "CURRENCY_EXCHANGE_RATE");
    api_call.setFieldValue(Url::FROM_CURRENCY, symbol);
    api_call.setFieldValue(Url::TO_CURRENCY, market);

    std::string data = api_call.curlQuery();

    nlohmann::json json =
        nlohmann::json::parse(data)["Realtime Currency Exchange Rate"];

    std::time_t timestamp = toUnixTimestamp(json["6. Last Refreshed"]);

    std::vector<float> exchange_data = {
        std::stof(std::string(json["5. Exchange Rate"])),
        std::stof(std::string(json["8. Bid Price"])),
        std::stof(std::string(json["9. Ask Price"]))};

    return {symbol, market, timestamp, exchange_data};
}
const std::vector<std::string> Crypto::series_function = {
    "DIGITAL_CURRENCY_DAILY", "DIGITAL_CURRENCY_WEEKLY",
    "DIGITAL_CURRENCY_MONTHLY"};

} // namespace avapi