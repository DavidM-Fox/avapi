#ifndef AVAPI_H
#define AVAPI_H
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>

namespace avapi {

class Quote {
public:
    Quote(std::string symbol, std::string api_key);
    ~Quote();

    void getIntraday(std::string interval);
    void getDaily(int last_n);
    void getWeekly(int last_n);
    void getMonthly(int last_n);
    void getGlobalQuote();

    std::string m_symbol;

private:
    std::string m_api_key;

    void download(const std::string &t_url, const std::string &file_name);

    std::string m_fileName;
    std::string m_url;
};

// Helper Functions
bool string_replace(std::string &str, const std::string from,
                    const std::string to);
std::string readFirstLineFromFile(const std::string &file_name);

} // namespace avapi
#endif // AVAPI_H