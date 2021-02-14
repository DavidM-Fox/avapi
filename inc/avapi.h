#ifndef AVAPI_H
#define AVAPI_H
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace avapi {

typedef std::pair<std::time_t, std::vector<float>> time_pair;
typedef std::vector<time_pair> time_series;

enum class function { DAILY, WEEKLY, MONTHLY };

class Quote {
public:
    Quote(std::string symbol, std::string api_key);
    ~Quote();

    time_series getIntradayDataSeries(const std::string &interval = "15min");
    time_series getTimeDataSeries(const function &func,
                                  const size_t &n_last = 30);
    time_pair getGlobalQuote();

    std::string m_symbol;
    std::string m_api_key;

private:
    time_series parseIntradayCsv(const std::string &file_name);

    time_series parseTimeDataSeries(const std::string &file_name,
                                    const size_t &n_last);
};

// Helper Functions
void download(const std::string &t_url, const std::string &file_name);
bool string_replace(std::string &str, const std::string from,
                    const std::string to);
std::string readFirstLineFromFile(const std::string &file_name);
std::time_t dateTimeToUnixTimestamp(std::string input);
std::time_t dateToUnixTimestamp(std::string input);

} // namespace avapi
#endif // AVAPI_H