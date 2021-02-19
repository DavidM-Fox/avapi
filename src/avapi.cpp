#include "../inc/avapi.h"

namespace avapi {

// Quote class constructor
Quote::Quote(std::string symbol, std::string api_key)
    : m_symbol(symbol), m_api_key(api_key),
      m_url_base("https://www.alphavantage.co/query?function="),
      m_url_end("&symbol={symbol}&apikey={api}&datatype=csv"),
      m_urls(
          {m_url_base + "TIME_SERIES_INTRADAY&interval={interval}" + m_url_end,
           m_url_base + "TIME_SERIES_DAILY" + m_url_end,
           m_url_base + "TIME_SERIES_WEEKLY" + m_url_end,
           m_url_base + "TIME_SERIES_MONTHLY" + m_url_end}),
      m_fnames({"..\\..\\data\\intraday_{symbol}.csv",
                "..\\..\\data\\daily_{symbol}.csv",
                "..\\..\\data\\weekly_{symbol}.csv",
                "..\\..\\data\\monthly_{symbol}.csv"})
{
}

// Quote deconstructor
Quote::~Quote() {}

// Get a "func" time_series from last_n with each time_pair being:
//
// (time_t unix_timestamp, vector[open, high, low, close, volume])
//
// func: avapi::INTRADAY/DAILY/WEEKLY/MONTHLY
// last_n: last number of data rows since today (default=30)
// interval: Interval for avapi::INTRADAY (default="30min")
time_series Quote::getTimeSeries(const function &func, const size_t &last_n,
                                 const std::string &interval)
{
    // Create url query for getting said time series
    std::string url = this->m_urls[func];
    if (func == avapi::INTRADAY) {
        stringReplace(url, "{interval}", interval);
    }
    stringReplace(url, "{symbol}", this->m_symbol);
    stringReplace(url, "{api}", this->m_api_key);

    // Create filename for time series csv
    std::string file_name = this->m_fnames[func];
    stringReplace(file_name, "{symbol}", this->m_symbol);

    // Download and return parsed csv file as avapi::time_series
    downloadCsv(url, file_name);
    return parseTimeSeriesCsv(file_name, last_n);
}

// Get time_pair for latest global quote:
//
// (time_t unix_timestamp,
// vector[open,high,low,price,volume,prevClose,change,change%])
time_pair Quote::getGlobalQuote()
{
    // Create url query for getting global quote
    std::string url = this->m_url_base + "GLOBAL_QUOTE" + this->m_url_end;
    stringReplace(url, "{symbol}", this->m_symbol);
    stringReplace(url, "{api}", this->m_api_key);

    // Create url query for global quote csv
    std::string file_name = "..\\..\\data\\global_quote_{symbol}.csv";
    stringReplace(file_name, "{symbol}", this->m_symbol);

    // Download csv file for global quote
    downloadCsv(url, file_name);
    rapidcsv::Document doc(file_name, rapidcsv::LabelParams(0, 0));
    std::vector<float> data = doc.GetRow<float>(this->m_symbol);

    // Erase final data column and put at the beginning as a unix_timestamp
    data.erase(data.begin() + 5);
    time_pair global_quote = std::make_pair(
        toUnixTimestamp(doc.GetColumn<std::string>("latestDay")[0]), data);

    // Return global quote as a avapi::time_pair
    return global_quote;
}

// Create time_series from a downloaded alpha vantage csv
time_series parseTimeSeriesCsv(const std::string &file_name,
                               const size_t &last_n)
{
    rapidcsv::Document doc(file_name);

    std::vector<std::string> date_col = doc.GetColumn<std::string>(0);
    size_t n_data = date_col.size();

    if (last_n > n_data) {
        std::cout << "Error: Not enough data rows in in file for last_n series"
                  << '\n';
        time_series fail;
        return fail;
    }

    std::vector<float> open = doc.GetColumn<float>(1);
    std::vector<float> high = doc.GetColumn<float>(2);
    std::vector<float> low = doc.GetColumn<float>(3);
    std::vector<float> close = doc.GetColumn<float>(4);
    std::vector<float> volume = doc.GetColumn<float>(5);

    time_series series;
    std::string format = "%Y-%m-%d %H:%M:%S";
    // ss >> std::get_time(&t, "%Y-%m-%d");

    for (size_t i = 0; i < last_n; ++i) {

        std::vector<float> data;

        data.push_back(open[i]);
        data.push_back(high[i]);
        data.push_back(low[i]);
        data.push_back(close[i]);
        data.push_back(volume[i]);

        series.push_back(std::make_pair(toUnixTimestamp(date_col[i]), data));
    }
    std::reverse(series.begin(), series.end());
    return series;
}

// Helper Functions -----------------------

// Curls .csv data file using "t_url" and saves it to .csv "file_name"
void downloadCsv(const std::string &t_url, const std::string &file_name)
{
    const char *url = t_url.c_str();
    const char *file = file_name.c_str();
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(file, "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

// Replaces "from" to "to" within "str"
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

// Reads the first line from provided file, used mainly for reading an api key
// .txt file
std::string readFirstLineFromFile(const std::string &file_name)
{
    std::string api_key;
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::getline(file, api_key);
        file.close();
    }
    else {
        std::cout << "Unable to open file: " << file_name;
    }
    return api_key;
}

// Converts date + time string "%Y-%m-%d %H:%M:%S" to Unix Timestamp (seconds
// since epoch)
std::time_t toUnixTimestamp(const std::string &input)
{
    std::tm t{};
    std::istringstream ss(input);
    std::string format = "%Y-%m-%d %H:%M:%S";
    ss >> std::get_time(&t, format.c_str());
    return mktime(&t);
}

// Prints the data within time_series to console
void printSeries(const time_series &series)
{
    for (auto &pair : series) {
        std::cout << pair.first << ':';
        for (auto &val : pair.second) {
            std::cout << ' ' << val;
        }
        std::cout << '\n';
    }
}

// Prints the data within time_pair to console
void printPair(const time_pair &pair)
{
    std::cout << pair.first << ':';
    for (auto &val : pair.second) {
        std::cout << ' ' << val;
    }
    std::cout << '\n';
}
} // namespace avapi