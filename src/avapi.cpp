#include <algorithm>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <nlohmann/json.hpp>
#include <regex>
#include "avapi.h"
#include "rapidcsv.h"

namespace avapi {

/// @brief   ApiCall Class constructor
/// @param   api_key The Alpha Vantage API key to use
ApiCall::ApiCall(const std::string &api_key) : m_apiKey(api_key)
{
    if (api_key == "")
        throw std::invalid_argument(
            "Empty API Key fed to constructor ApiCall()");
}

/// @brief   Set a field and value within m_fieldValueMap
/// @param   field The Api::field
/// @param   value The value to be set as a string
void ApiCall::setApiField(const API::field &field, const std::string &value)
{
    std::string key = m_apiFieldVec[field];
    m_fieldValueMap[key] = value;
}

/// @brief   Get a value from a field within m_fieldValueMap
/// @param   field The API::field
/// @returns The value corresponding to the field parameter
std::string ApiCall::getApiField(const API::field &field)
{
    std::string key = m_apiFieldVec[field];
    if (m_fieldValueMap.count(key) == 1)
        return m_fieldValueMap.find(key)->second;

    std::string error = "'avapi::ApiCall::getApiField: Field \"" + key +
                        "\" not found within m_fieldValueMap";
    throw std::exception(error.c_str());
}

/// @brief Clear every field within m_fieldValueMap
void ApiCall::clearApiFields() { m_fieldValueMap.clear(); }

/// @brief Construct an API query url using m_fieldValueMap
/// @returns An Alpha Vantage API query URL
std::string ApiCall::buildApiUrl()
{
    std::string url{"https://www.alphavantage.co/query?"};

    for (auto &pair : m_fieldValueMap) {
        url += pair.first + pair.second;
    }
    return url + "&apikey=" + m_apiKey;
}

/// @brief   Curls data from a specific API query URL
/// @param   url The API query URL to be curled
/// @returns The data as an std::string
std::string ApiCall::queryApiUrl(const std::string &url)
{
    CURL *curl;
    CURLcode res;
    std::string data;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    return data;
}

/// @brief Field strings for m_fieldValueMap
const std::vector<std::string> ApiCall::m_apiFieldVec{
    "&function=", "&symbol=",        "&interval=",    "&adjusted=",  "&market=",
    "&datatype=", "&from_currency=", "&to_currency=", "&outputsize="};

/// @brief   Callback function for CURLOPT_WRITEFUNCTION
/// @param   ptr The downloaded chunk members
/// @param   size Member memory size
/// @param   nmemb Number of members
/// @param   data Current running chunk for data appension
/// @returns The current running chunk's realsize
size_t ApiCall::WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                    void *data)
{
    size_t realsize = size * nmemb;

    std::string *mem = reinterpret_cast<std::string *>(data);
    mem->append(static_cast<char *>(ptr), realsize);
    return realsize;
}

/// @brief   avapi::Stock constructor
/// @param   symbol The stock symbol of interest
/// @param   api_key The Alpha Vantage API key to use
Stock::Stock(const std::string &symbol, const std::string &api_key)
    : ApiCall(api_key), m_symbol(symbol)
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
TimeSeries Stock::getTimeSeries(const avapi::series::type &type,
                                const bool &adjusted,
                                const std::string &interval)
{
    // Clear API fields for new configuration
    clearApiFields();

    std::string title;

    // Set API function field
    switch (type) {
    case series::INTRADAY:
        setApiField(API::FUNCTION, "TIME_SERIES_INTRADAY");
        setApiField(API::INTERVAL, interval);
        if (adjusted) {
            setApiField(API::ADJUSTED, "true");
            title =
                "Intraday Time Series (Adjusted, interval = " + interval + ')';
        }
        else {
            setApiField(API::ADJUSTED, "false");
            title =
                "Intraday Time Series (Non-Adjusted, interval = " + interval +
                ')';
        }
        break;

    case series::DAILY:
        if (adjusted) {
            setApiField(API::FUNCTION, "TIME_SERIES_DAILY_ADJUSTED");
            title = "Daily Time Series (Adjusted)";
        }

        else {
            setApiField(API::FUNCTION, "TIME_SERIES_DAILY");
            title = "Daily Time Series (Non-Adjusted)";
        }
        break;

    case series::WEEKLY:
        if (adjusted) {
            setApiField(API::FUNCTION, "TIME_SERIES_WEEKLY_ADJUSTED");
            title = "Weekly Time Series (Adjusted)";
        }
        else {
            setApiField(API::FUNCTION, "TIME_SERIES_WEEKLY");
            title = "Weekly Time Series (Non-Adjusted)";
        }
        break;

    case series::MONTHLY:
        if (adjusted) {
            setApiField(API::FUNCTION, "TIME_SERIES_WEEKLY_ADJUSTED");
            title = "Monthly Time Series (Adjusted)";
        }
        else {
            setApiField(API::FUNCTION, "TIME_SERIES_WEEKLY");
            title = "Monthly Time Series (Non-Adjusted)";
        }
        break;

    default:
        std::invalid_argument ex("Incorrect avapi::Stock::function argument");
        throw ex;
    }

    // Set other needed API fields
    setApiField(API::SYMBOL, m_symbol);
    setApiField(API::OUTPUT_SIZE, m_outputSize);
    setApiField(API::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()));
    series.setSymbol(m_symbol);
    series.setType(type);
    series.setTitle(m_symbol + ": " + title);
    return series;
}

/// @brief Set the output size for Alpha Vantage API calls
/// @param size The output size: "compact" or "full" (default = "compact")
void Stock::setOutputSize(const std::string &size) { m_outputSize = size; }

/// @brief   Return the symbol's latest global quote
/// @returns The symbol's global quote as an avapi::GlobalQuote object
GlobalQuote Stock::getGlobalQuote()
{
    // Clear API fields for new configuration
    clearApiFields();

    setApiField(API::FUNCTION, "GLOBAL_QUOTE");
    setApiField(API::SYMBOL, m_symbol);
    setApiField(API::DATA_TYPE, "csv");

    // Download csv data for global quote
    std::stringstream csv(queryApiUrl(buildApiUrl()));

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

    GlobalQuote global_quote(timestamp, data_f);
    return global_quote;
}

/// @brief   avapi::Crypto constructor
/// @param   symbol The crypto symbol of interest
/// @param   api_key The Alpha Vantage API key to use
Crypto::Crypto(const std::string &symbol, const std::string &api_key)
    : ApiCall(api_key), m_symbol(symbol)
{
    // Force symbol to be capitalized
    std::transform(m_symbol.begin(), m_symbol.end(), m_symbol.begin(),
                   ::toupper);
    setOutputSize("compact");
}

/// @brief   Get an avapi::TimeSeries for a crypto symbol of interest.
/// @param   type enum avapi::series::type for TimeSeries type
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::TimeSeries: [open,high,low,close,volume]
TimeSeries Crypto::getTimeSeries(const series::type &type,
                                 const std::string &market)
{
    // Clear API fields for new configuration
    clearApiFields();

    series::type series_type = type;

    // Set API function field
    switch (type) {
    case series::INTRADAY:
        series_type = series::DAILY;

    case series::DAILY:
        setApiField(API::FUNCTION, "DIGITAL_CURRENCY_DAILY");
        break;

    case series::WEEKLY:
        setApiField(API::FUNCTION, "DIGITAL_CURRENCY_WEEKLY");
        break;

    case series::MONTHLY:
        setApiField(API::FUNCTION, "DIGITAL_CURRENCY_MONTHLY");
        break;

    default:
        std::invalid_argument ex("Incorrect avapi::Crypto::function argument");
        throw ex;
    }

    // Set other needed API fields
    setApiField(API::SYMBOL, m_symbol);
    setApiField(API::MARKET, market);
    setApiField(API::OUTPUT_SIZE, m_outputSize);
    setApiField(API::DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()), true);
    series.setSymbol(m_symbol);
    return series;
}

/// @brief Set the output size for Alpha Vantage API calls
/// @param size The output size: "compact" or "full" (default = "compact")
void Crypto::setOutputSize(const std::string &size) { m_outputSize = size; }

/// @brief   Get the current exchange rate for a specific market
/// @param   market The exchange market (default = "USD")
/// @returns An avapi::ExchangeRate object: [Exchange, Bid, Ask]
ExchangeRate Crypto::getExchangeRate(const std::string &market)
{
    // Clear API fields for new configuration
    clearApiFields();
    setApiField(API::FUNCTION, "CURRENCY_EXCHANGE_RATE");
    setApiField(API::FROM_CURRENCY, m_symbol);
    setApiField(API::TO_CURRENCY, market);

    std::string data = queryApiUrl(buildApiUrl());

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

/// @brief TimeSeries constructor
/// @param data A vector of avapi::TimePair data
TimeSeries::TimeSeries(const std::vector<avapi::TimePair> &data) : m_data(data)
{
}

/// @brief TimeSeries default constructor
TimeSeries::TimeSeries() {}

/// @brief Push an avapi::TimePair into the avapi::TimeSeries
/// @param pair An avapi::TimePair to be pushed back
void TimeSeries::pushBack(const TimePair &pair) { m_data.push_back(pair); }

/// @brief Set the avapi::TimeSeries' type
/// @param type avapi::series::type
void TimeSeries::setType(const series::type &type) { m_type = type; }

/// @brief Set the avapi::TimeSeries' symbol
/// @param symbol A symbol as an std::string
void TimeSeries::setSymbol(const std::string &symbol) { m_symbol = symbol; }

/// @brief Set the avapi::TimeSeries' title
/// @param symbol A title as an std::string
void TimeSeries::setTitle(const std::string &title) { m_title = title; }

/// @brief Get the avapi::TimeSeries' row count
/// @return size_t row count
const size_t TimeSeries::rowCount() { return m_nRows; }

/// @brief Get the avapi::TimeSeries' column count
/// @return size_t column count
const size_t TimeSeries::colCount() { return m_nCols; }

/// @brief Set the avapi::TimeSeries' column headers
/// @param headers A vector of header strings
void TimeSeries::setHeaders(const std::vector<std::string> &headers)
{
    m_headers = headers;
}

/// @brief   Reverses the avapi::TimeSeries' data, useful for when the data is
/// desired to be plotted
void TimeSeries::reverseData()
{
    // Data coming from Alpha Vantage is reversed (Dates are reversed)
    std::reverse(m_data.begin(), m_data.end());
}

/// @brief push formated avapi::TimeSeries' contents to ostream
std::ostream &operator<<(std::ostream &os, const TimeSeries &series)
{
    size_t sep_count = 0;
    size_t volume_index = 0;
    size_t width = 15;

    for (auto &heading : series.m_headers) {
        os << std::setw(width) << heading;
        sep_count += width;
    }

    std::string separator(sep_count + 6, '-');

    os << '\n' << separator << '\n';

    for (auto &pair : series.m_data) {
        os << std::setw(width) << std::right << pair.m_time;
        for (auto &value : pair.m_data) {
            os << std::setw(width) << std::right << std::fixed
               << std::setprecision(2) << value;
        }
        os << '\n';
    }
    return os;
}

/// @brief GlobalQuote constructor
/// @param timestamp A Unix timestamp
/// @param data std::vector<float> ordered: [Open, High, Low, Close, Volume,
/// Prev_Close, Change, Change%]
GlobalQuote::GlobalQuote(const std::time_t &timestamp,
                         const std::vector<float> &data)
    : timestamp(timestamp), open(data[0]), high(data[1]), low(data[2]),
      close(data[3]), volume(data[4]), close_previous(data[5]), change(data[6]),
      change_percent(data[7]),
      headers({"Open", "High", "Low", "Close", "Volume", "Prev_Close", "Change",
               "Change%"})
{
}

/// @brief ExchangeRate constructor
/// @param from Symbol converting from
/// @param to Symbol converting to
/// @param timestamp A Unix timestamp
/// @param data std::vector<float> ordered: [Exchange Rate, Bid Price, Ask
/// Price]
ExchangeRate::ExchangeRate(const std::string &from, const std::string &to,
                           const std::time_t &t, const std::vector<float> &data)
    : from_symbol(from), to_symbol(to), timestamp(t), exchange_rate(data[0]),
      bid_price(data[1]), ask_price(data[2])
{
}

/// @brief   Returns an avapi::time_series created from a csv file
/// @param   file file path of the csv file to parse
/// @param   crypto Wheter the csv data is from a crypto symbol
/// @returns avapi::TimeSeries
TimeSeries parseCsvFile(const std::string &file_path, const bool &crypto)
{

    // Try to open file first
    std::ifstream file(file_path.c_str());
    if (file.is_open()) {
        file.close();
    }
    else {
        std::string error =
            "'avapi::parseCsvFile': \"" + file_path + "\" cannot be opened.";
        throw std::exception(error.c_str());
    }

    // File exists, create rapidcsv::Document object
    rapidcsv::Document doc(file_path);
    size_t n_rows = doc.GetRowCount();

    // Test if rapidcsv failed to parse file
    if (n_rows == 0) {
        std::string error = "'avapi::parseCsvFile': \"" + file_path +
                            "\" is of invalid csv type.";
        throw std::exception(error.c_str());
    }

    // Successful parse (Cells could still be invalid...)
    // TimePairVec series;
    TimeSeries series;
    if (crypto) {

        //// Remove useless columns
        // Volume = Market Cap (Dont know why.. free version?)
        doc.RemoveColumn(10);

        // Redundant USD columns
        doc.RemoveColumn(8);
        doc.RemoveColumn(7);
        doc.RemoveColumn(6);
        doc.RemoveColumn(5);

        for (size_t i = 0; i < n_rows; ++i) {
            std::vector<std::string> row = doc.GetRow<std::string>(i);

            // Transform vector into floats, skip timestamp column
            std::vector<float> data;
            std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                           [](std::string &value) { return std::stof(value); });
            TimePair pair(toUnixTimestamp(row[0]), data);
            series.pushBack(pair);
        }
    }
    else {
        for (size_t i = 0; i < n_rows; ++i) {
            std::vector<std::string> row = doc.GetRow<std::string>(i);

            // Transform vector into floats, skip timestamp column
            std::vector<float> data;
            std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                           [](std::string &value) { return std::stof(value); });

            TimePair pair(toUnixTimestamp(row[0]), data);
            series.pushBack(pair);
        }
    }

    std::vector<std::string> headers = doc.GetColumnNames();
    series.setHeaders(headers);
    return series;
}

/// @brief   Returns an avapi::time_series created from a csv std::string
/// @param   data An csv std::string object
/// @param   crypto Wheter the csv data is from a crypto symbol
/// @returns avapi::TimeSeries
TimeSeries parseCsvString(const std::string &data, const bool &crypto)
{
    // Create rapidcsv::Document object from csv string
    std::stringstream sstream(data);
    rapidcsv::Document doc(sstream);
    size_t n_rows = doc.GetRowCount();

    // Test if data is really a JSON response
    if (n_rows <= 2 && isJsonString(data)) {
        nlohmann::json parser = nlohmann::json::parse(data);
        std::string error =
            "'avapi::parseCsvString': Json Response:" + parser.dump(4);
        throw std::exception(error.c_str());
    }

    // Successful parse (Cells could still be invalid...)
    TimeSeries series;
    if (crypto) {
        //// Remove useless columns
        // Volume = Market Cap (Dont know why.. free version?)
        doc.RemoveColumn(10);

        // Redundant USD columns
        doc.RemoveColumn(8);
        doc.RemoveColumn(7);
        doc.RemoveColumn(6);
        doc.RemoveColumn(5);

        for (size_t i = 0; i < n_rows; ++i) {
            std::vector<std::string> row = doc.GetRow<std::string>(i);

            // Transform vector into floats, skip timestamp column
            std::vector<float> data;
            std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                           [](std::string &value) { return std::stof(value); });
            TimePair pair(toUnixTimestamp(row[0]), data);
            series.pushBack(pair);
        }
    }
    else {
        for (size_t i = 0; i < n_rows; ++i) {
            std::vector<std::string> row = doc.GetRow<std::string>(i);

            // Transform vector into floats, skip timestamp column
            std::vector<float> data;
            std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                           [](std::string &value) { return std::stof(value); });

            TimePair pair(toUnixTimestamp(row[0]), data);
            series.pushBack(pair);
        }
    }

    std::vector<std::string> headers = doc.GetColumnNames();
    series.setHeaders(headers);
    return series;
}

/// @brief   Replaces a substring within a given string
/// @param   str The given string
/// @param   from The substring being replaced by "to"
/// @param   to The substring replacing "from"
/// @returns A success/fail bool
bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to)
{
    size_t start = str.find(from);
    if (start == std::string::npos) {
        return false;
    }
    str.replace(start, from.length(), to);
    return true;
}

/// @brief   Reads the first line from a given file
/// @param   file_path File to be read's path
/// @returns first line as an std::string
std::string readFirstLineFromFile(const std::string &file_path)
{
    std::string api_key = "";
    std::ifstream file(file_path.c_str());

    if (file.is_open()) {
        std::getline(file, api_key);
        file.close();
    }
    else {

        std::exception ex(
            std::string("\"" + file_path + "\" cannot be opened").c_str());
        throw ex;
    }
    return api_key;
}

/// @brief   Converts date + time string "%Y-%m-%d %H:%M:%S" to Unix Timestamp
/// (seconds since unic epoch)
/// @param   input The input string to be converted
/// @returns The time as an std::time_t unix timestamp
std::time_t toUnixTimestamp(const std::string &input)
{
    std::tm t{};
    std::istringstream ss(input);
    std::string format = "%Y-%m-%d %H:%M:%S";
    ss >> std::get_time(&t, format.c_str());
    return mktime(&t);
}

/// @brief   Test if a string is in JSON format
/// @param   data The string to be tested
/// @returns bool
bool isJsonString(const std::string &data)
{
    try {
        nlohmann::json parser = nlohmann::json::parse(data);
    }
    catch (...) {
        return false;
    }
    return true;
}
} // namespace avapi