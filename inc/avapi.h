#ifndef AVAPI_H
#define AVAPI_H
#include "rapidcsv.h"
#include <algorithm>
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace avapi {

typedef std::pair<std::time_t, std::vector<float>> time_pair;
typedef std::vector<time_pair> time_series;

// enum for std::vector<std::string> m_urls;
enum url_func { INTRADAY = 0, DAILY, WEEKLY, MONTHLY };

// enum for std::vector<std::string> m_urls;
enum csv_type { STDSTRING = 0, FILEPATH };

// Main class of avapi library "Quote"
class Quote {
public:
    Quote(std::string symbol, std::string api_key);
    ~Quote();

    time_series getTimeSeries(const url_func &func,
                              const size_t &last_n_rows = 0,
                              const std::string &interval = "15min");
    time_pair getGlobalQuote();

private:
    static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb,
                                      void *data);
    std::string downloadCsv(const std::string &t_url);
    std::string m_symbol;
    std::string m_api_key;

    // Alpha Vantage API url placeholders
    std::string m_url_base;
    std::string m_url_end;
    std::vector<std::string> m_urls;
};

// avapi::Quote Helper functions
bool stringReplace(std::string &str, const std::string &from,
                   const std::string &to);
std::string readFirstLineFromFile(const std::string &file_path);
std::time_t toUnixTimestamp(const std::string &input);
void printSeries(const time_series &series);
void printPair(const time_pair &pair);
time_series parseCsvFile(const std::string &file,
                         const size_t &last_n_rows = 0);
time_series parseCsvString(const std::string &data,
                           const size_t &last_n_rows = 0);

extern std::vector<float> null_vector;
extern avapi::time_pair null_pair;
extern avapi::time_series null_series;

} // namespace avapi
#endif // AVAPI_H