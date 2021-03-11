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
Crypto::Crypto()
{
    this->symbol = "";
    api_call.api_key = "";
    api_call.output_size = "compact";
}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
/// @param   key The Alpha Vantage API key to use
Crypto::Crypto(const std::string &symbol, const std::string &key)
    : symbol(symbol), api_key(key), api_call(key)
{
    api_call.output_size = "compact";
}

/// @brief  Crypto deconstructor
Crypto::~Crypto()
{
    if (exchange_rate != nullptr) {
        delete exchange_rate;
    }
}

void Crypto::setApiKey(const std::string &key)
{
    this->api_key = key;
    api_call.api_key = key;
    if (exchange_rate != nullptr) {
        exchange_rate->setApiKey(key);
    }
}

void Crypto::setSymbol(const std::string &symbol)
{
    this->symbol = symbol;
    if (exchange_rate != nullptr) {
        exchange_rate->setSymbol(symbol);
    }
}

ExchangeRate *Crypto::Exchange(const std::string &market)
{
    if (exchange_rate == nullptr) {
        exchange_rate = new ExchangeRate(symbol, market, api_key);
        return exchange_rate;
    }
    else {
        return exchange_rate;
    }
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

const std::vector<std::string> Crypto::series_function = {
    "DIGITAL_CURRENCY_DAILY", "DIGITAL_CURRENCY_WEEKLY",
    "DIGITAL_CURRENCY_MONTHLY"};

} // namespace avapi