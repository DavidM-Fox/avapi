#include <algorithm>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "avapi.h"
#include "rapidcsv.h"

namespace avapi {

/**
 * @brief   ApiCall Default Class constructor
 * @param   symbol The stock symbol of interest
 * @param   api_key The Alpha Vantage API key to use
 */
ApiCall::ApiCall() {}

/**
 * @brief   ApiCall Class constructor
 * @param   symbol The stock symbol of interest
 * @param   api_key The Alpha Vantage API key to use
 */
ApiCall::ApiCall(std::string symbol, std::string api_key)
    : m_symbol(symbol), m_apiKey(api_key)
{
}

std::string ApiCall::buildApiUrl(const function &func,
                                 const std::string &interval,
                                 const std::string &config)
{
    std::string func = "&function=" + m_funcStrVec[func];
    std::string key = "&apikey=" + m_apiKey;

    switch (func) {
    case TIME_SERIES_INTRADAY:
        break;

    case TIME_SERIES_INTRADAY_EXTENDED:
        break;

    case TIME_SERIES_DAILY:
        break;

    case TIME_SERIES_DAILY_ADJUSTED:
        break;

    case TIME_SERIES_WEEKLY:
        break;

    case TIME_SERIES_WEEKLY_ADJUSTED:
        break;

    case TIME_SERIES_MONTHLY_ADJUSTED:
        break;

    case GLOBAL_QUOTE:
        break;

    case OVERVIEW:
        break;

    case CURRENCY_EXCHANGE_RATE:
        break;

    case CRYPTO_RATING:
        break;

    case DIGITAL_CURRENCY_DAILY:
        break;

    case DIGITAL_CURRENCY_WEEKLY:
        break;

    case DIGITAL_CURRENCY_MONTHLY:
        break;
    }
    std::string func = "&function=" + function;
    std::string key = "&apikey=" + m_apiKey;

    std::string url = m_urlBase;

    stringReplace(url, "{func}", function);
    stringReplace(url, "{symbol}", m_symbol);
    stringReplace(url, "{api}", m_apiKey);
    if (interval == "") {
        stringReplace(url, "{interval}", "");
    }
    else {
        std::string replace = "&interval=" + interval;
        stringReplace(url, "{interval}", replace);
    }
    stringReplace(url, "{config}", config);
    return url;
}

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

/**
 * @brief Base url for all Alpha Vantage API calls
 * "https://www.alphavantage.co/query?function={func}&symbol={symbol}&apikey={api}{interval}{config}"
 * @param {func} e.g TIME_SERIES_INTRADAY
 * @param {symbol} e.g TSLA
 * @param {api} Alpha Vantage API Key
 * @param {interval} Only used when func = TIME_SERIES_INTRADAY
 * @param {config} Additional configuration (datatype, market)
 */
const std::string
    ApiCall::m_urlBase("https://www.alphavantage.co/query?function=");

//{func}&symbol={symbol}&apikey={api}{interval}{config}");

const std::vector<std::string> ApiCall::m_funcStrVec{
    "TIME_SERIES_INTRADAY",
    "TIME_SERIES_INTRADAY_EXTENDED",
    "TIME_SERIES_DAILY",
    "TIME_SERIES_DAILY_ADJUSTED",
    "TIME_SERIES_WEEKLY",
    "TIME_SERIES_WEEKLY_ADJUSTED",
    "TIME_SERIES_MONTHLY_ADJUSTED",
    "GLOBAL_QUOTE",
    "OVERVIEW",
    "CURRENCY_EXCHANGE_RATE",
    "CRYPTO_RATING",
    "DIGITAL_CURRENCY_DAILY",
    "DIGITAL_CURRENCY_WEEKLY",
    "DIGITAL_CURRENCY_MONTHL"};

/**
 * @brief   Stock Class constructor
 * @param   symbol The stock symbol of interest
 * @param   api_key The Alpha Vantage API key to use
 */
Stock::Stock(const std::string &symbol, const std::string &api_key)
    : ApiCall(symbol, api_key)
{
}

/**
 * @brief   Get an intraday time series for a symbol of interest.
 * @param   interval The intraday data interval ("1min", "5min", "15min",
 * "30min", or "60min", default = "30min")
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series with the data vector being ordered [open,
 * high, low, close, volume]
 */
time_series Stock::getIntradaySeries(const std::string &interval,
                                     const size_t &last_n_rows)
{
    // Create url query
    std::string url =
        buildApiUrl("TIME_SERIES_INTRADAY", interval, "&datatype=csv");

    // Download csv data for time series
    std::string csv_string = queryApiUrl(url);
    return parseCsvString(csv_string, last_n_rows);
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
time_series Stock::getDailySeries(const bool &adjusted,
                                  const size_t &last_n_rows)
{
    // Create url query
    std::string function = "TIME_SERIES_DAILY";
    if (adjusted) {
        function += "_ADJUSTED";
    }

    std::string url = buildApiUrl(function, "", "&datatype=csv");

    // Download csv data for time series
    std::string csv_string = queryApiUrl(url);
    return parseCsvString(csv_string, last_n_rows);
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
time_series Stock::getWeeklySeries(const bool &adjusted,
                                   const size_t &last_n_rows)
{
    // Create url query
    std::string function = "TIME_SERIES_WEEKLY";
    if (adjusted) {
        function += "_ADJUSTED";
    }

    std::string url = buildApiUrl(function, "", "&datatype=csv");

    // Download csv data for time series
    std::string csv_string = queryApiUrl(url);
    return parseCsvString(csv_string, last_n_rows);
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
time_series Stock::getMonthlySeries(const bool &adjusted,
                                    const size_t &last_n_rows)
{
    // Create url query
    std::string function = "TIME_SERIES_MONTHLY";
    if (adjusted) {
        function += "_ADJUSTED";
    }

    std::string url = buildApiUrl(function, "", "&datatype=csv");

    // Download csv data for time series
    std::string csv_string = queryApiUrl(url);
    return parseCsvString(csv_string, last_n_rows);
}

/**
 * @brief   Returns an avapi::time_pair containing the latest global quote:
 * @returns latest global quote as an avapi::time_pair:
 * [open,high,low,price,volume,prevClose,change,change%]
 */
time_pair Stock::getGlobalQuote()
{
    // Download csv data for global quote
    std::stringstream csv(
        queryApiUrl(buildApiUrl("GLOBAL_QUOTE", "", "&datatype=csv")));

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

    return std::make_pair(timestamp, data_f);
}

/**
 * @brief   Crypto Class constructor
 * @param   symbol The stock symbol of interest
 * @param   api_key The Alpha Vantage API key to use
 */
Crypto::Crypto(const std::string &symbol, const std::string &api_key)
    : ApiCall(symbol, api_key)
{
}

/**
 * @brief   Get a daily time series for a cryptocurrency of interest.
 * @param   market The exchange market (default = "USD")
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 */
time_series Crypto::getDailySeries(const std::string &market,
                                   const size_t &last_n_rows)
{
    // Create url query
    std::string url = buildApiUrl("DIGITAL_CURRENCY_DAILY", "",
                                  "&market=" + market + "&datatype=csv");

    // Download csv data
    std::string csv_string = queryApiUrl(url);
    return parseCsvString(csv_string, last_n_rows);
}

/**
 * @brief   Get a daily time series for a cryptocurrency of interest.
 * @param   market The exchange market (default = "USD")
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 */
time_series Crypto::getWeeklySeries(const std::string &market,
                                    const size_t &last_n_rows)
{
    // Create url query
    std::string url = buildApiUrl("DIGITAL_CURRENCY_WEEKLY", "",
                                  "&market=" + market + "&datatype=csv");

    // Download csv data
    std::string csv_string = queryApiUrl(url);
    return parseCsvString(csv_string, last_n_rows);
}

/**
 * @brief   Get a weekly time series for a cryptocurrency of interest.
 * @param   market The exchange market (default = "USD")
 * @param   last_n_rows Last number of rows to get (default = 0 or all)
 * @returns An avapi::time_series: [open,high,low,close,volume])
 */
time_series Crypto::getMonthlySeries(const std::string &market,
                                     const size_t &last_n_rows)
{
    // Create url query
    std::string url = buildApiUrl("DIGITAL_CURRENCY_MONTHLY", "",
                                  "&market=" + market + "&datatype=csv");

    // Download csv data
    std::string csv_string = queryApiUrl(url);
    return parseCsvString(csv_string, last_n_rows);
}

/**
 * @brief   Get the current exchange rate for a specific market
 * @param   market The exchange market (default = "USD")
 * @returns An avapi::time_pair: [Exchange_Rate, Bid Price, Ask Price]
 */
time_pair Crypto::getExchangeRate(const std::string &market)
{
    std::string url = buildApiUrl("CURRENCY_EXCHANGE_RATE", "");
}

/**
 * @brief   Returns an avapi::time_series created from a csv file
 * @param   file file path of the csv file to parse
 * @param   last_n_rows last number of rows to return
 * @returns avapi::time_series
 */
time_series parseCsvFile(const std::string &file, const size_t &last_n_rows)
{
    // Create document object from csv file path
    rapidcsv::Document doc;
    size_t n_rows = 0;

    try {
        doc.Load(file);
        n_rows = doc.GetRowCount();
        if (n_rows == 0)
            throw std::exception("CSV file contents are invalid.");
    }
    catch (std::exception &ex) {
        std::cerr << "Exception caught within avapi::parseCsvFile() '"
                  << ex.what() << "'.\n";
        std::cerr << "File path or contents of '" << file
                  << "' may be invalid. Returning a null avapi::time_series "
                     "object.\n";
        return null_series;
    }

    // Ensure safe iteration over data.
    if (last_n_rows == 0) {
    }
    else if (n_rows > last_n_rows) {
        n_rows = last_n_rows;
    }

    std::vector<std::string> row = doc.GetRow<std::string>(0);
    size_t n_rowData = row.size();

    // Iterate over n_rows, parsing data into the avapi::time_series
    avapi::time_series series;
    for (size_t i = 0; i < n_rows; ++i) {
        std::vector<std::string> row = doc.GetRow<std::string>(i);

        std::vector<float> data;
        for (size_t j = 1; j < n_rowData; ++j) {
            data.push_back(std::stof(row[j]));
        }

        series.push_back(std::make_pair(toUnixTimestamp(row[0]), data));
    }
    return series;
}

/**
 * @brief   Returns an avapi::time_series created from a csv std::string
 * @param   data An csv std::string object
 * @param   last_n_rows last number of rows to return. Returns every row if
 * parameter is greater than document row count.
 * @returns avapi::time_series
 */
time_series parseCsvString(const std::string &data, const size_t &last_n_rows)
{

    // Create document object from csv string
    std::stringstream sstream(data);
    rapidcsv::Document doc;
    size_t n_rows = 0;

    try {
        doc.Load(sstream);
        n_rows = doc.GetRowCount();
        if (n_rows == 0)
            throw std::exception("'const std::string &data is invalid.'");
    }
    catch (std::exception &ex) {
        std::cerr << "Exception caught within avapi::parseCsvString(): "
                  << ex.what() << '\n';
        std::cerr << "Returning a null avapi::time_series "
                     "object.\n";

        return null_series;
    }

    // Ensure safe iteration over data.
    if (last_n_rows == 0) {
    }
    else if (n_rows > last_n_rows) {
        n_rows = last_n_rows;
    }

    std::vector<std::string> row = doc.GetRow<std::string>(0);
    size_t n_rowData = row.size();

    // Iterate over n_rows, parsing data into the avapi::time_series
    avapi::time_series series;
    for (size_t i = 0; i < n_rows; ++i) {
        std::vector<std::string> row = doc.GetRow<std::string>(i);

        std::vector<float> data;
        for (size_t j = 1; j < n_rowData; ++j) {
            data.push_back(std::stof(row[j]));
        }

        series.push_back(std::make_pair(toUnixTimestamp(row[0]), data));
    }
    return series;
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
    if (file.is_open()) {
        std::getline(file, api_key);
        file.close();
    }
    else {
        std::cout << "Unable to open file: " << file_path;
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

/**
 * @brief   Prints an avapi::time_series' data to console
 * @param   series The avapi::time_series to be printed
 * @param   adjusted Whether or not the data is adjusted (default = false)
 */
void printSeries(const time_series &series, const bool &adjusted)
{
    std::vector<std::string> header;
    std::string separator;

    if (adjusted) {
        header = {"open",      "high",   "low",      "close",
                  "adj_close", "volume", "dividend", "split_coeff"};
        separator = "----------------------------------------------------------"
                    "------------------------------------------------";

        std::cout << std::setw(12) << std::right << "Timestamp";
        for (auto &label : header) {
            std::cout << std::setw(12) << std::right << label;
        }

        std::cout << '\n' << separator << '\n';

        for (auto &pair : series) {
            std::cout << std::setw(12) << std::right << pair.first;

            for (size_t i = 0; i < 5; ++i) {
                std::cout << std::setw(12) << std::right << std::fixed
                          << std::setprecision(2) << pair.second[i];
            }
            std::cout << std::setw(12) << std::right << std::setprecision(0)
                      << pair.second[5];

            std::cout << std::setw(12) << std::right << std::fixed
                      << std::setprecision(2) << pair.second[6];

            std::cout << std::setw(12) << std::right << std::fixed
                      << std::setprecision(2) << pair.second[7];
            std::cout << '\n';
        }
    }
    else {
        header = {"Open", "High", "Low", "Close", "Volume"};
        separator = "----------------------------------------------------------"
                    "--------------";

        std::cout << std::setw(12) << std::right << "Timestamp";
        for (auto &label : header) {
            std::cout << std::setw(12) << std::right << label;
        }

        std::cout << '\n' << separator << '\n';

        for (auto &pair : series) {
            std::cout << std::setw(12) << std::right << pair.first;

            for (size_t i = 0; i < 4; ++i) {
                std::cout << std::setw(12) << std::right << std::fixed
                          << std::setprecision(2) << pair.second[i];
            }
            std::cout << std::setw(12) << std::right << std::setprecision(0)
                      << pair.second[4];
            std::cout << '\n';
        }
    }
}

/**
 * @brief   Prints an avapi::global_quote to console
 * @param   quote The avapi::global_quote to be printed
 */
void printGlobalQuote(const global_quote &pair)
{
    std::vector<std::string> header{"Open",   "High",   "Low",
                                    "Close",  "Volume", "Prev_Close",
                                    "Change", "Change%"};

    std::string separator =
        "----------------------------------------------------------------------"
        "--------------------------------------";

    std::cout << std::setw(12) << std::right << "Timestamp";

    for (auto &label : header) {
        std::cout << std::setw(12) << std::right << label;
    }

    std::cout << '\n' << separator << '\n';

    std::cout << std::setw(12) << std::right << pair.first;

    for (size_t i = 0; i < 4; ++i) {
        std::cout << std::setw(12) << std::right << std::fixed
                  << std::setprecision(2) << pair.second[i];
    }
    std::cout << std::setw(12) << std::right << std::setprecision(0)
              << pair.second[4];

    std::cout << std::setw(12) << std::right << std::fixed
              << std::setprecision(2) << pair.second[5];

    std::cout << std::setw(12) << std::right << std::fixed
              << std::setprecision(2) << pair.second[6];

    std::cout << std::setw(12) << std::right << std::fixed
              << std::setprecision(2) << pair.second[7];
    std::cout << '\n';
}

/**
 * @brief   Reverses the orde of an avapi::time_series
 * @param   series The avapi::time_series to be reversed
 */
void reverseTimeSeries(avapi::time_series &series)
{
    // Data coming from Alpha Vantage is reversed (Dates are reversed)
    std::reverse(series.begin(), series.end());
}

/// @brief Null object for exception handlers
static const std::vector<float> null_vector{NULL};
/// @brief Null object for exception handlers
static const avapi::time_pair null_pair{NULL, null_vector};
/// @brief Null object for exception handlers
static const avapi::time_series null_series{null_pair};
} // namespace avapi