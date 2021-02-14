#ifndef AVAPI_H
#define AVAPI_H
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

namespace avapi {

class Quote {
public:
    Quote(std::string symbol, std::string api_key);
    ~Quote();

    std::vector<std::vector<float>> getIntraday(std::string interval);
    std::vector<std::vector<float>> getDaily(int last_n);
    std::vector<std::vector<float>> getWeekly(int last_n);
    std::vector<std::vector<float>> getMonthly(int last_n);
    std::vector<std::vector<float>> getGlobalQuote();

    std::vector<std::time_t> getTimeCol();

    std::string m_symbol;

private:
    std::string m_api_key;

    void download(const std::string &t_url, const std::string &file_name);
    std::vector<std::vector<float>> parse(const std::string &file_name);

    std::string m_fileName;
    std::string m_url;
    std::vector<std::time_t> m_timeCol;
    std::vector<float> m_volCol;
};

// Helper Functions
bool string_replace(std::string &str, const std::string from,
                    const std::string to);
std::string readFirstLineFromFile(const std::string &file_name);

} // namespace avapi
#endif // AVAPI_H