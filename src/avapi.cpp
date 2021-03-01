#include <algorithm>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include "avapi.h"
#include "rapidcsv.h"

namespace avapi {

// Static helper variables
static const std::string
    api_urlBase("https://www.alphavantage.co/query?function=");
static const std::string
    api_urlEnd("&symbol={symbol}&apikey={api}&datatype=csv");
static const std::vector<std::string> api_urlFuncs{
    api_urlBase + "TIME_SERIES_INTRADAY" + api_urlEnd,
    api_urlBase + "TIME_SERIES_DAILY" + api_urlEnd,
    api_urlBase + "TIME_SERIES_WEEKLY" + api_urlEnd,
    api_urlBase + "TIME_SERIES_MONTHLY" + api_urlEnd};
static const std::vector<std::string> api_urlIntervals{"1min", "5min", "15min",
                                                       "30min", "60min"};
static const std::vector<float> null_vector{NULL};
static const avapi::time_pair null_pair = std::make_pair(NULL, null_vector);
static const avapi::time_series null_series{null_pair};

/**
 * @brief   Class constructor
 * @param   symbol The stock symbol of interest
 * @param   api_key The Alpha Vantage API key to use
 */
Stock::Stock(std::string symbol, std::string api_key)
    : m_symbol(symbol), m_api_key(api_key)
{
}

/**
 * @brief   Class Deconstructor
 */
Stock::~Stock() {}

/**
 * @brief   Returns a "func" time_series from last_n_rows;
 * @param   func The avapi::function to use
 * @param   last_n_rows Last number of rows to get - default = 0 (all)
 * @param   interval Interval for avapi::INTRADAY - default = "30min"
 * @returns An avapi::time_series with each pair.second =
 * std::vector<float>[open, high, low, close, volume]
 */
time_series Stock::getTimeSeries(const api_func &func,
                                 const size_t &last_n_rows,
                                 const api_interval &interval)
{
    // Create url query for time series
    std::string url = api_urlFuncs[func];
    if (func == INTRADAY) {
        url += "&interval=" + api_urlIntervals[interval];
    }
    stringReplace(url, "{symbol}", m_symbol);
    stringReplace(url, "{api}", m_api_key);

    // Download csv data for global quote
    std::string csv_string = downloadCsv(url);
    return parseCsvString(csv_string, last_n_rows);
}

/**
 * @brief   Returns an avapi::time_pair containing the latest global quote:
 * @returns latest global quote as an avapi::time_pair with pair.second =
 * std::vector<float>[open,high,low,price,volume,prevClose,change,change]
 */
time_pair Stock::getGlobalQuote()
{
    // Create url query for getting global quote
    std::string url = api_urlBase + "GLOBAL_QUOTE" + api_urlEnd;
    stringReplace(url, "{symbol}", m_symbol);
    stringReplace(url, "{api}", m_api_key);

    // Download csv data for global quote
    std::stringstream csv(downloadCsv(url));

    // Get global quote row from csv std::string
    rapidcsv::Document doc(csv);
    std::vector<std::string> data = doc.GetRow<std::string>(0);

    // Erase symbol field
    data.erase(data.begin());

    // Convert latestDay field and then erase it
    std::time_t timestamp = toUnixTimestamp(data[5]);
    data.erase(data.begin() + 5);

    // Convert to vector of floats
    std::vector<float> data_f(data.size());
    transform(data.begin(), data.end(), data_f.begin(),
              [](std::string const &val) { return std::stof(val); });

    return std::make_pair(timestamp, data_f);
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

    // Iterate over n_rows, parsing data into the avapi::time_series
    avapi::time_series series;
    for (size_t i = 0; i < n_rows; ++i) {
        std::vector<std::string> row = doc.GetRow<std::string>(i);

        std::vector<float> data;
        data.push_back(std::stof(row[1]));
        data.push_back(std::stof(row[2]));
        data.push_back(std::stof(row[3]));
        data.push_back(std::stof(row[4]));
        data.push_back(std::stof(row[5]));

        series.push_back(std::make_pair(toUnixTimestamp(row[0]), data));
    }

    // Data coming from Alpha Vantage is reversed (Dates are reversed)
    std::reverse(series.begin(), series.end());
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

    // Iterate over n_rows, parsing data into the avapi::time_series
    avapi::time_series series;
    for (size_t i = 0; i < n_rows; ++i) {
        std::vector<std::string> row = doc.GetRow<std::string>(i);

        std::vector<float> data;
        data.push_back(std::stof(row[1]));
        data.push_back(std::stof(row[2]));
        data.push_back(std::stof(row[3]));
        data.push_back(std::stof(row[4]));
        data.push_back(std::stof(row[5]));

        series.push_back(std::make_pair(toUnixTimestamp(row[0]), data));
    }

    // Data coming from Alpha Vantage is reversed (Dates are reversed)
    std::reverse(series.begin(), series.end());
    return series;
}

/**
 * @brief   Callback function for CURLOPT_WRITEFUNCTION
 * @param   ptr The downloaded chunk members
 * @param   size Member memory size
 * @param   nmemb Number of members
 * @param   data Current running chunk for data appension
 * @returns The current running chunk's realsize
 */
size_t Stock::WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                  void *data)
{
    size_t realsize = size * nmemb;

    std::string *mem = reinterpret_cast<std::string *>(data);
    mem->append(static_cast<char *>(ptr), realsize);
    return realsize;
}

/**
 * @brief   Curls csv data and uses WriteMemoryCallback to store data
 * @param   t_url The url to be curled
 * @returns The csv data as an std::string
 */
std::string Stock::downloadCsv(const std::string &t_url)
{
    CURL *curl;
    CURLcode res;
    std::string data;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, t_url.c_str());
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
 * @brief   Replaces a substring within a given string
 * @param   str The given string
 * @param   from The substring being replaced by "to"
 * @param   to The substring replacing "from"
 * @returns Success bool
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
 * @returns std::time_t timestamp
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
 */
void print(const time_series &series)
{
    for (auto &pair : series) {
        std::cout << pair.first << ':';
        for (auto &val : pair.second) {
            std::cout << ' ' << val;
        }
        std::cout << '\n';
    }
}

/**
 * @brief   Prints an avapi::time_pair's data to console
 * @param   pair The avapi::time_pair to be printed
 */
void print(const time_pair &pair)
{
    std::cout << pair.first << ':';
    for (auto &val : pair.second) {
        std::cout << ' ' << val;
    }
    std::cout << '\n';
}

} // namespace avapi