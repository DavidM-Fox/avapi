#include <algorithm>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include "rapidcsv.h"
#include "avapi/ApiCall.hpp"
#include "avapi/misc.hpp"
#include "avapi/TimeSeries.hpp"
#include "avapi/Crypto.hpp"
#include "avapi/crypto/ExchangeRate.hpp"

namespace avapi {

/// @brief   avapi::Crypto Default Constructor
Crypto::Crypto() : symbol(""), ApiCall("") { output_size = "compact"; }

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
/// @param   key The Alpha Vantage API key to use
Crypto::Crypto(const std::string &symbol, const std::string &key)
    : symbol(symbol), ApiCall(key)
{
    output_size = "compact";
}

/// @brief  Crypto deconstructor
Crypto::~Crypto()
{
    if (exchange_rate != nullptr) {
        delete exchange_rate;
    }
    if (health_index != nullptr) {
        delete health_index;
    }
}

/// @brief Set the Alpha Vantage API key for Crypto and its components
/// @param key Alpha Vantage API key
void Crypto::setApiKey(const std::string &key)
{
    this->api_key = key;
    if (exchange_rate != nullptr) {
        exchange_rate->api_key = key;
    }
    if (health_index != nullptr) {
        health_index->api_key = key;
    }
}

/// @brief Set the cryptocurrency of interest for Crypto and its components
/// @param symbol cryptocurrency symbol
void Crypto::setSymbol(const std::string &symbol)
{
    this->symbol = symbol;
    if (exchange_rate != nullptr) {
        exchange_rate->from_symbol = symbol;
    }
    if (health_index != nullptr) {
        health_index->symbol = symbol;
    }
}

/// @brief Return an avapi::ExchangeRate* for this Crypto object
/// @param market Exchange Market e.g. ("USD")
ExchangeRate *Crypto::Exchange(const std::string &market)
{
    if (exchange_rate == nullptr) {
        exchange_rate = new ExchangeRate(symbol, market, api_key);
        return exchange_rate;
    }
    return exchange_rate;
}

/// @brief Return an avapi::HealthIndex* for this Crypto object
/// @param market Exchange Market e.g. ("USD")
HealthIndex *Crypto::Health()
{
    if (health_index == nullptr) {
        health_index = new HealthIndex(symbol, api_key);
        return health_index;
    }
    return health_index;
}

/// @brief   Set the TimeSeries output size from Alpha Vantage
/// @param   size enum class SeriesSize [COMPACT, FULL]
void Crypto::setOutputSize(const SeriesSize &size)
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
TimeSeries Crypto::getTimeSeries(const SeriesType &type,
                                 const std::string &market)
{
    resetQuery();

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

const std::vector<std::string> Crypto::series_function = {
    "", "DIGITAL_CURRENCY_DAILY", "DIGITAL_CURRENCY_WEEKLY",
    "DIGITAL_CURRENCY_MONTHLY"};

} // namespace avapi