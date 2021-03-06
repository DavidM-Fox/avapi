#include <algorithm>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>
#include <regex>
#include "avapi.h"
#include "rapidcsv.h"

namespace avapi {

/**
 * @brief   ApiCall Class constructor
 * @param   api_key The Alpha Vantage API key to use
 */
ApiCall::ApiCall(const std::string &api_key) : m_apiKey(api_key)
{
    if (api_key == "")
        throw "Empty api_key provided to ApiCall constructor";
}

/**
 * @brief   Set a field and value within m_fieldValueMap
 * @param   field The ApiCall::api_field
 * @param   value The value to be set as a string
 */
void ApiCall::setApiField(const api_field &field, const std::string &value)
{
    std::string key = m_apiFieldVec[field];
    m_fieldValueMap[key] = value;
}

/**
 * @brief   Get a value from a field within m_fieldValueMap
 * @param   field The ApiCall::api_field
 * @returns The value corresponding to the field parameter
 */
std::string ApiCall::getApiField(const api_field &field)
{
    std::string key = m_apiFieldVec[field];
    if (m_fieldValueMap.count(key) == 1)
        return m_fieldValueMap.find(key)->second;

    std::string error = "Avapi Exception: Field \"" + key +
                        "\" not found within m_fieldValueMap";
    throw error;
}

/**
 * @brief Clear every field within m_fieldValueMap
 */
void ApiCall::clearApiFields() { m_fieldValueMap.clear(); }

/**
 * @brief Construct the url to be used within queryApiUrl() using the fields and
 * values within m_fieldValueMap
 * @returns An Alpha Vantage API query URL
 */
std::string ApiCall::buildApiUrl()
{
    std::string url = m_urlBase;

    for (auto &pair : m_fieldValueMap) {
        url += pair.first + pair.second;
    }
    return url + "&apikey=" + m_apiKey;
}

/**
 * @brief   Curls data from a specific API url query
 * @param   url The url to be curled
 * @returns The data as an std::string
 */
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

const std::string ApiCall::m_urlBase{"https://www.alphavantage.co/query?"};
const std::vector<std::string> ApiCall::m_apiFieldVec{
    "&function=", "&symbol=",        "&interval=",    "&adjusted=",  "&market=",
    "&datatype=", "&from_currency=", "&to_currency=", "&outputsize="};

/**
 * @brief   Callback function for CURLOPT_WRITEFUNCTION
 * @param   ptr The downloaded chunk members
 * @param   size Member memory size
 * @param   nmemb Number of members
 * @param   data Current running chunk for data appension
 * @returns The current running chunk's realsize
 */
size_t ApiCall::WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                    void *data)
{
    size_t realsize = size * nmemb;

    std::string *mem = reinterpret_cast<std::string *>(data);
    mem->append(static_cast<char *>(ptr), realsize);
    return realsize;
}

/**
 * @brief   Stock Class constructor
 * @param   symbol The stock symbol of interest
 * @param   api_key The Alpha Vantage API key to use
 */
Stock::Stock(const std::string &symbol, const std::string &api_key)
    : ApiCall(api_key), m_symbol(symbol)
{
    // Force symbol to be capitalized
    std::transform(m_symbol.begin(), m_symbol.end(), m_symbol.begin(),
                   ::toupper);
}

/**
 * @brief   Get an intraday time series for a symbol of interest.
 * @param   interval The intraday data interval ("1min", "5min", "15min",
 * "30min", or "60min", default = "30min")
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series with the data vector being ordered [open,
 * high, low, close, volume]
 */
TimeSeries Stock::getIntradaySeries(const std::string &interval,
                                    const bool &is_adjusted)
{
    // Clear and configure API fields
    clearApiFields();
    setApiField(FUNCTION, "TIME_SERIES_INTRADAY");
    setApiField(SYMBOL, m_symbol);
    setApiField(INTERVAL, interval);

    if (is_adjusted)
        setApiField(ADJUSTED, "true");
    else
        setApiField(ADJUSTED, "false");

    setApiField(OUTPUT_SIZE, "compact");
    setApiField(DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()));
    series.setSymbol(m_symbol);
    return series;
}

/**
 * @brief   Get a daily time series for a symbol of interest.
 * @param   adjusted Whether or not the data should have adjusted values
 * (default = false)
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 * or an adjusted avapi::time_series:
 * [open,high,low,close,adjusted_close,volume,dividend_amount,split_coefficient])
 */
TimeSeries Stock::getDailySeries(const bool &is_adjusted)
{
    // Clear and configure API fields
    clearApiFields();

    if (is_adjusted)
        setApiField(FUNCTION, "TIME_SERIES_DAILY_ADJUSTED");
    else
        setApiField(FUNCTION, "TIME_SERIES_DAILY");

    setApiField(SYMBOL, m_symbol);
    setApiField(OUTPUT_SIZE, "compact");
    setApiField(DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()));
    series.setSymbol(m_symbol);
    return series;
}

/**
 * @brief   Get a weekly time series for a symbol of interest.
 * @param   adjusted Whether or not the data should have adjusted values
 * (default = false)
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 * or an adjusted avapi::time_series:
 * [open,high,low,close,adjusted_close,volume,dividend_amount,split_coefficient])
 */
TimeSeries Stock::getWeeklySeries(const bool &is_adjusted)
{
    // Configure API url
    clearApiFields();

    if (is_adjusted)
        setApiField(FUNCTION, "TIME_SERIES_WEEKLY_ADJUSTED");
    else
        setApiField(FUNCTION, "TIME_SERIES_WEEKLY");

    setApiField(SYMBOL, m_symbol);
    setApiField(OUTPUT_SIZE, "compact");
    setApiField(DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()));
    series.setSymbol(m_symbol);
    return series;
}

/**
 * @brief   Get a monthly time series for a symbol of interest.
 * @param   adjusted Whether or not the data should have adjusted values
 * (default = false)
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 * or an adjusted avapi::time_series:
 * [open,high,low,close,adjusted_close,volume,dividend_amount,split_coefficient])
 */
TimeSeries Stock::getMonthlySeries(const bool &is_adjusted)
{
    // Configure API url
    clearApiFields();

    if (is_adjusted)
        setApiField(FUNCTION, "TIME_SERIES_MONTHLY_ADJUSTED");
    else
        setApiField(FUNCTION, "TIME_SERIES_MONTHLY");

    setApiField(SYMBOL, m_symbol);
    setApiField(OUTPUT_SIZE, "compact");
    setApiField(DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()));
    series.setSymbol(m_symbol);
    return series;
}

/**
 * @brief   Returns an avapi::time_pair containing the latest global quote:
 * @returns latest global quote as an avapi::time_pair:
 * [open,high,low,price,volume,prevClose,change,change%]
 */
GlobalQuote Stock::getGlobalQuote()
{
    // Configure API url
    clearApiFields();
    setApiField(FUNCTION, "GLOBAL_QUOTE");
    setApiField(SYMBOL, m_symbol);
    setApiField(DATA_TYPE, "csv");

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

/**
 * @brief   Crypto Class constructor
 * @param   symbol The stock symbol of interest
 * @param   api_key The Alpha Vantage API key to use
 */
Crypto::Crypto(const std::string &symbol, const std::string &api_key)
    : ApiCall(api_key), m_symbol(symbol)
{
    // Force symbol to be capitalized
    std::transform(m_symbol.begin(), m_symbol.end(), m_symbol.begin(),
                   ::toupper);
}

/**
 * @brief   Get a daily time series for a cryptocurrency of interest.
 * @param   market The exchange market (default = "USD")
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 */
TimeSeries Crypto::getDailySeries(const std::string &market)
{
    // Configure API url
    clearApiFields();

    setApiField(FUNCTION, "DIGITAL_CURRENCY_DAILY");
    setApiField(SYMBOL, m_symbol);
    setApiField(MARKET, market);
    setApiField(OUTPUT_SIZE, "compact");
    setApiField(DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()));
    series.setSymbol(m_symbol);
    return series;
}

/**
 * @brief   Get a daily time series for a cryptocurrency of interest.
 * @param   market The exchange market (default = "USD")
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 */
TimeSeries Crypto::getWeeklySeries(const std::string &market)
{
    // Configure API url
    clearApiFields();

    setApiField(FUNCTION, "DIGITAL_CURRENCY_WEEKLY");
    setApiField(SYMBOL, m_symbol);
    setApiField(MARKET, market);
    setApiField(OUTPUT_SIZE, "compact");
    setApiField(DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()));
    series.setSymbol(m_symbol);
    return series;
}

/**
 * @brief   Get a weekly time series for a cryptocurrency of interest.
 * @param   market The exchange market (default = "USD")
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 */
TimeSeries Crypto::getMonthlySeries(const std::string &market)
{
    // Configure API url
    clearApiFields();
    setApiField(FUNCTION, "DIGITAL_CURRENCY_MONTHLY");
    setApiField(SYMBOL, m_symbol);
    setApiField(MARKET, market);
    setApiField(OUTPUT_SIZE, "compact");
    setApiField(DATA_TYPE, "csv");

    // Download, parse, and create TimeSeries from csv data
    TimeSeries series = parseCsvString(queryApiUrl(buildApiUrl()));
    series.setSymbol(m_symbol);
    return series;
}

/**
 * @brief   Get the current exchange rate for a specific market
 * @param   market The exchange market (default = "USD")
 * @returns An avapi::time_pair: [Exchange_Rate, Bid Price, Ask Price]
 */
ExchangeRate Crypto::getExchangeRate(const std::string &market)
{
    // Configure API url
    clearApiFields();
    setApiField(FUNCTION, "CURRENCY_EXCHANGE_RATE");
    setApiField(FROM_CURRENCY, m_symbol);
    setApiField(TO_CURRENCY, market);

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

TimeSeries::TimeSeries(const TimePairVec &data, const std::string &symbol,
                       const series_type &type, const bool &is_adjusted,
                       const bool &is_crypto, const std::string &market)
    : m_data(data), m_nRows(data.size()), m_nCols(data[0].m_data.size()),
      m_symbol(symbol), m_type(type), m_isAdjusted(is_adjusted),
      m_isCrypto(is_crypto), m_market(market)
{
}

TimeSeries::TimeSeries(const TimePairVec &data, const std::string &symbol,
                       const bool &is_crypto)
    : m_data(data), m_nRows(data.size()), m_nCols(data[0].m_data.size()),
      m_symbol(symbol)
{
}

TimeSeries::TimeSeries(const TimePairVec &data, const series_type &type)
    : m_data(data), m_type(type)
{
}

void TimeSeries::pushBack(const TimePair &pair) { m_data.push_back(pair); }

void TimeSeries::setSymbol(const std::string &symbol) { m_symbol = symbol; }

/**
 * @brief Get the TimeSeries' row count
 * @return size_t row count
 */
const size_t TimeSeries::rowCount() { return m_nRows; }

/**
 * @brief Get the TimeSeries' column count
 * @return size_t column count
 */
const size_t TimeSeries::colCount() { return m_nCols; }

const bool TimeSeries::isCrypto() { return m_isCrypto; }

/**
 * @brief Set the TimeSeries' column headers
 */
void TimeSeries::setHeaders(const std::vector<std::string> &headers)
{
    m_headers = headers;
}

std::ostream &operator<<(std::ostream &os, const TimeSeries &series)
{
    size_t sep_count = 0;
    size_t volume_index = 0;
    size_t width = 15;

    // Check if need to print crypto headers
    if (series.m_isCrypto) {

        // print headers, ignoring extra columns
        for (size_t i = 0; i < 5; ++i) {
            os << std::setw(width) << series.m_headers[i];
            sep_count += width;
        }

        os << std::setw(width) << "volume";
        sep_count += width;

        os << '\n' << std::string(sep_count, '-') << '\n';

        for (auto &pair : series.m_data) {
            os << std::setw(width) << std::right << pair.m_time;
            for (size_t i = 0; i < 4; ++i) {
                os << std::setw(width) << std::right << std::fixed
                   << std::setprecision(2) << pair.m_data[i];
            }
            os << std::setw(width) << std::right << std::fixed
               << std::setprecision(2) << pair.m_data[4] << '\n';
        }
    }
    else {

        for (auto &heading : series.m_headers) {
            os << std::setw(12) << heading;
            sep_count += 12;
        }

        std::string separator(sep_count, '-');

        os << '\n' << separator << '\n';

        for (auto &pair : series.m_data) {
            os << std::setw(12) << std::left << pair.m_time;
            for (auto &value : pair.m_data) {
                os << std::setw(12) << std::left << std::fixed
                   << std::setprecision(2) << value;
            }
            os << '\n';
        }
    }

    return os;
}

GlobalQuote::GlobalQuote(const std::time_t &timestamp,
                         const std::vector<float> &data)
    : timestamp(timestamp), open(data[0]), high(data[1]), low(data[2]),
      close(data[3]), volume(data[4]), close_previous(data[5]), change(data[6]),
      change_percent(data[7]),
      headers({"Open", "High", "Low", "Close", "Volume", "Prev_Close", "Change",
               "Change%"})
{
}

ExchangeRate::ExchangeRate(const std::string &from, const std::string &to,
                           const std::time_t &t, const std::vector<float> &data)
    : from_symbol(from), to_symbol(to), timestamp(t), exchange_rate(data[0]),
      bid_price(data[1]), ask_price(data[2])
{
}

/**
 * @brief   Returns an avapi::time_series created from a csv file
 * @param   file file path of the csv file to parse
 * @param   last_n_rows last number of rows to return
 * @returns avapi::time_series
 */
TimeSeries parseCsvFile(const std::string &file)
{
    // Create document object from csv file path
    rapidcsv::Document doc(file);
    size_t n_rows = doc.GetRowCount();

    try {
        if (n_rows == 0) {
            std::string error = "Invalid CSV File \"" + file + "\"";
            throw error;
        }
    }
    catch (std::string &ex) {
        std::cerr << "Exception caught within avapi::parseCsvFile(), returning "
                     "a null TimeSeries: "
                  << ex << ".\n";
        TimePair pair(-1, {0});
        TimeSeries er_series({pair}, TimeSeries::EMPTY);
        return er_series;
    }

    std::vector<std::string> headers = doc.GetColumnNames();
    TimeSeries::series_type type = discernSeriesType(headers);

    TimePairVec series;
    if (type == TimeSeries::CRYPTO) {
        // TimePairVec is Crypto, iterate over every row and neccessary cols
        for (size_t i = 0; i < n_rows; ++i) {

            std::vector<std::string> row = doc.GetRow<std::string>(i);

            // Transform vector into floats, skip timestamp col and uneeded cols
            std::vector<float> data;
            for (size_t i = 1; i < 5; ++i) {
                data.push_back(std::stof(row[i]));
            }
            data.push_back(std::stof(row[9]));
            TimePair pair(toUnixTimestamp(row[0]), data);
            series.push_back(pair);
        }
    }
    else {
        // TimePairVec is not Crypto, iterate over each row/col
        for (size_t i = 0; i < n_rows; ++i) {

            std::vector<std::string> row = doc.GetRow<std::string>(i);

            // Transform row vector into floats (skip timestamp col)
            std::vector<float> data;
            std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                           [](std::string &value) { return std::stof(value); });

            TimePair pair(toUnixTimestamp(row[0]), data);
            series.push_back(pair);
        }
    }

    TimeSeries time_series(series, type);
    time_series.setHeaders(headers);

    return time_series;
}

/**
 * @brief   Returns an avapi::time_series created from a csv std::string
 * @param   data An csv std::string object
 * @param   last_n_rows last number of rows to return. Returns every row if
 * parameter is greater than document row count.
 * @returns avapi::time_series
 */
TimeSeries parseCsvString(const std::string &data)
{
    // Create document object from csv string
    std::stringstream sstream(data);
    rapidcsv::Document doc(sstream);
    size_t n_rows = doc.GetRowCount();

    // Test if data is really a JSON response
    if (n_rows <= 2) {
        try {
            if (isJsonString(data))
                throw "Alpha Vantage JSON Response:";
        }
        catch (const char *ex) {
            std::cerr << "Exception caught within avapi::parseCsvString(), "
                         "returning a null time_series: "
                      << ex << '\n';
            nlohmann::json parser = nlohmann::json::parse(data);
            std::cerr << parser.dump(4);
            TimePair pair(-1, {0});
            TimeSeries er_series({pair}, TimeSeries::EMPTY);
            return er_series;
        }
    }

    // Get Series type through regex function
    std::vector<std::string> headers = doc.GetColumnNames();
    TimeSeries::series_type type = discernSeriesType(headers);

    // Iterate over each row, parsing data into a TimePairVec
    TimePairVec series;
    for (size_t i = 0; i < n_rows; ++i) {

        std::vector<std::string> row = doc.GetRow<std::string>(i);

        // Transform row into floats (skip timestamp col)
        std::vector<float> data;
        std::transform(row.begin() + 1, row.end(), std::back_inserter(data),
                       [](std::string &value) { return std::stof(value); });

        TimePair pair(toUnixTimestamp(row[0]), data);
        series.push_back(pair);
    }

    TimeSeries time_series(series, type);
    time_series.setHeaders(headers);
    return time_series;
}

/**
 * @brief   Replaces a substring within a given string
 * @param   str The given string
 * @param   from The substring being replaced by "to"
 * @param   to The substring replacing "from"
 * @returns A success/fail bool
 */
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

/**
 * @brief   Reads the first line from a given file
 * @param   file_path File to be read's path
 * @returns first line as an std::string
 */
std::string readFirstLineFromFile(const std::string &file_path)
{
    std::string api_key = "";
    std::ifstream file(file_path);
    try {
        if (file.is_open()) {
            std::getline(file, api_key);
            file.close();
        }
        else {
            std::string error = "Cannot open file \"" + file_path + "\"";
            throw error;
        }
    }
    catch (std::string &ex) {
        std::cerr << "Exception caught within readFirstLineFromFile(): " << ex
                  << ", returning an empty API Key.\n";
        return "";
    }
    return api_key;
}

/**
 * @brief   Converts date + time string "%Y-%m-%d %H:%M:%S" to Unix Timestamp
 * (seconds since unic epoch)
 * @param   input The input string to be converted
 * @returns The time as an std::time_t timestamp
 */
std::time_t toUnixTimestamp(const std::string &input)
{
    std::tm t{};
    std::istringstream ss(input);
    std::string format = "%Y-%m-%d %H:%M:%S";
    ss >> std::get_time(&t, format.c_str());
    return mktime(&t);
}

TimeSeries::series_type
discernSeriesType(const std::vector<std::string> &headers)
{
    // Combine all headers into one string to discern csv type
    std::string str;
    for (auto &header : headers) {
        str += header;
    }

    // Remove whitespace from combined str
    str.erase(remove(str.begin(), str.end(), ' '), str.end());

    std::regex nonadjusted_stock("timestampopenhighlowclosevolume");
    std::regex adjusted_stock(
        "timestampopenhighlowcloseadjustedclosevolumedividendamount");
    std::regex crypto(
        "timestampopen\\([a-zA-z]*\\)high\\([a-zA-z]*\\)low\\([a-zA-z]*\\)"
        "close\\([a-zA-z]*\\)open\\([a-zA-z]*\\)high\\([a-zA-z]*\\)low\\([a-"
        "zA-z]*\\)close\\([a-zA-z]*\\)volumemarketcap\\([a-zA-Z]*\\)");

    // Discern TimeSeries type from header match
    if (std::regex_match(str, nonadjusted_stock)) {
        return TimeSeries::NON_ADJUSTED;
    }
    else if (std::regex_match(str, adjusted_stock)) {
        return TimeSeries::ADJUSTED;
    }
    else if (std::regex_match(str, crypto)) {
        return TimeSeries::CRYPTO;
    }
}

/**
 * @brief   Reverses the orde of an avapi::time_series
 * @param   series The avapi::time_series to be reversed
 */
void reverseTimeSeries(TimePairVec &series)
{
    // Data coming from Alpha Vantage is reversed (Dates are reversed)
    std::reverse(series.begin(), series.end());
}

/**
 * @brief   Test if a string is in JSON format
 * @param   data The string to be tested
 * @returns bool
 */
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