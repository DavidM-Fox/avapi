#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include "avapi/ApiCall.hpp"
#include "avapi/misc.hpp"
#include "avapi/Container/TimeSeries.hpp"
#include "avapi/Crypto/Pricing.hpp"

namespace avapi {
/// @brief   avapi::CryptoPricing default constructor
CryptoPricing::CryptoPricing() : symbol(symbol), ApiCall("")
{
    output_size = "compact";
}

/// @brief   avapi::CryptoPricing constructor
/// @param   symbol The stock symbol of interest
/// @param   api_key The Alpha Vantage API key to use
CryptoPricing::CryptoPricing(const std::string &symbol, const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    output_size = "compact";
}

/// @brief   Set the TimeSeries output size from Alpha Vantage
/// @param   size enum class SeriesSize [COMPACT, FULL]
void CryptoPricing::setOutputSize(const SeriesSize &size)
{
    if (size == SeriesSize::COMPACT)
        output_size = "compact";
    else if (size == SeriesSize::FULL)
        output_size = "full";
}

/// @brief   Get an avapi::TimeSeries for a crypto symbol of interest.
/// @param   type enum class avapi::SeriesType
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::TimeSeries ordered [open,high,low,close,volume]
TimeSeries CryptoPricing::getTimeSeries(const SeriesType &type,
                                        const std::string &market)
{
    resetQuery();

    std::string function;
    SeriesType check = type;

    // Intraday not available from Alpha Vantage
    if (check == SeriesType::INTRADAY) {
        check = SeriesType::DAILY;
        std::cout << "avapi/Crypto.cpp: exception: "
                     "'avapi::Crypto::getTimeSeries': Intraday not available "
                     "from Alpha Vantage, returning a daily TimeSeries.\n";
    }

    function = series_function[static_cast<int>(check)];
    setFieldValue(Url::FUNCTION, function);

    // Set other needed API fields
    setFieldValue(Url::SYMBOL, symbol);
    setFieldValue(Url::MARKET, market);
    setFieldValue(Url::OUTPUT_SIZE, output_size);
    setFieldValue(Url::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(curlQuery(), true);
    series.symbol = symbol;
    series.type = check;
    series.is_adjusted = false;
    series.market = market;
    series.title = symbol + ": " + function;
    return series;
}

/// @brief Get an avapi::ExchangeRate for this Crypto object
/// @param market Exchange Market e.g. ("USD")
ExchangeRate CryptoPricing::Exchange(const std::string &market)
{
    return {symbol, market, api_key};
}

const std::vector<std::string> CryptoPricing::series_function = {
    "", "DIGITAL_CURRENCY_DAILY", "DIGITAL_CURRENCY_WEEKLY",
    "DIGITAL_CURRENCY_MONTHLY"};

} // namespace avapi